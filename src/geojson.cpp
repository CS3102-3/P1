// Proyeto 1
// Copyright © 2021 grupo 3
//
// p1 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// p1 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with p1.  If not, see <http://www.gnu.org/licenses/>.

#include <cmath>
#include <iostream>

#include <rapidjson/error/en.h>

#include "geojson.hpp"

const utec::geojson_parser::t_map_t utec::geojson_parser::to_map =
{
	{{state_t::in_feature, "properties"}, state_t::in_properties},
	{{state_t::in_feature, "geometry"}, state_t::in_geometry},
};

const utec::geojson_parser::t_map_t utec::geojson_parser::ta_map =
{
	{{state_t::in_json, "features"}, state_t::in_features},
	{{state_t::in_geometry, "coordinates"}, state_t::in_coordinates},
};

utec::geojson_parser::geojson_parser(FILE* file, std::function<bool(const bounding_box& box)> query_function):
	reader(),
	frs(file, buffer, sizeof(buffer)),
	query_function(query_function)
{
	if(!reader.Parse(frs, *this))
	{
		rapidjson::ParseErrorCode e = reader.GetParseErrorCode();
		std::cerr << rapidjson::GetParseError_En(e) << '\n';
		_good = false;
	}
};

bool utec::geojson_parser::good() const
{
	return _good && state == state_t::after_json;
}

bool utec::geojson_parser::StartObject()
{
	switch(state)
	{
		case state_t::before_json:
			state = state_t::in_json;
			break;

		case state_t::in_features:
			state = state_t::in_feature;
			reset_box();
			break;

		case state_t::in_feature:
		{
			auto it = to_map.find({state, last_key});
			if(it == to_map.end())
				return false;

			state = it->second;

			break;
		}

		default:
			return false;
	}

	return true;
}

bool utec::geojson_parser::EndObject(rapidjson::SizeType)
{
	switch(state)
	{
		case state_t::in_json:
			state = state_t::after_json;
			break;

		case state_t::in_feature:
			state = state_t::in_features;
			_good &= query_function(box);
			break;

		case state_t::in_properties:
			state = state_t::in_feature;
			break;

		case state_t::in_geometry:
			state = state_t::in_feature;
			break;

		default:
			return false;
	}
	return true;
}

bool utec::geojson_parser::StartArray()
{
	switch(state)
	{
		case state_t::in_json:
		case state_t::in_geometry:
		{
			auto it = ta_map.find({state, last_key});
			if(it == ta_map.end())
				return false;

			state = it->second;
			break;
		}

		case state_t::in_coordinates:
			state = state_t::in_coordinates2;
			break;

		case state_t::in_coordinates2:
			state = state_t::in_point;
			inserted_scalars = 0;
			break;

		default:
			return false;
	}
	return true;
}

bool utec::geojson_parser::EndArray(rapidjson::SizeType)
{
	switch(state)
	{
		case state_t::in_features:
			state = state_t::in_json;
			break;

		case state_t::in_coordinates:
			state = state_t::in_geometry;
			break;

		case state_t::in_coordinates2:
			state = state_t::in_coordinates;
			break;

		case state_t::in_point:
			state = state_t::in_coordinates2;
			break;

		default:
			return false;
	}
	return true;
}

bool utec::geojson_parser::Key(const char* str, rapidjson::SizeType size, bool)
{
	last_key = std::string_view(str, size);
	return true;
}

bool utec::geojson_parser::String(const char* str, rapidjson::SizeType size, bool)
{
	std::string_view str_v(str, size);

	switch(state)
	{
		case state_t::in_json:
			if(last_key == "type")
				if(str_v != "FeatureCollection")
					return false;
			break;

		case state_t::in_feature:
			if(last_key == "type")
				if(str_v != "Feature")
					return false;
			break;

		case state_t::in_geometry:
			if(last_key == "type")
				if(str_v != "Polygon")
					return false;
			break;

		default:
			return false;
	}

	return true;
}

bool utec::geojson_parser::Double(double d)
{
	switch(state)
	{
		case state_t::in_point:
			switch(inserted_scalars++)
			{
				case 0:
					add_longitude(d);
					break;

				case 1:
					add_latitude(d);
					break;

				default:
					return false;
			}
			break;

		default:
			return false;
	}
	return true;
}

bool utec::geojson_parser::Default()
{
	return false;
}

void utec::geojson_parser::reset_box()
{
	box.min_c = {INFINITY, INFINITY};
	box.max_c = {-INFINITY, -INFINITY};
}

void utec::geojson_parser::add_latitude(double latitude)
{
	box.min_c.latitude = std::min(box.min_c.latitude, latitude);
	box.max_c.latitude = std::max(box.max_c.latitude, latitude);
}

void utec::geojson_parser::add_longitude(double longitude)
{
	box.min_c.longitude = std::min(box.min_c.longitude, longitude);
	box.max_c.longitude = std::max(box.max_c.longitude, longitude);
}
