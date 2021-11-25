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

#pragma once

#include <climits>
#include <functional>
#include <iterator>
#include <string_view>
#include <map>

#include <rapidjson/filereadstream.h>
#include <rapidjson/reader.h>

namespace utec
{

struct coordinate
{
	double latitude;
	double longitude;
};

struct bounding_box
{
	coordinate min_c;
	coordinate max_c;
};

class geojson_parser:
	public rapidjson::BaseReaderHandler<rapidjson::UTF8<>, geojson_parser>
{
private:
	enum class state_t
	{
		before_json,
		in_json,
		after_json,
		in_features,
		in_feature,
		in_properties,
		in_geometry,
		in_coordinates,
		in_coordinates2,
		in_point
	};

	using t_map_t = std::map<std::pair<state_t, std::string_view>, state_t>;

	/// Contains state x key -> state transitions for objects.
	static const t_map_t to_map;

	/// Contains state x key -> state transitions for arrays.
	static const t_map_t ta_map;

	char                      buffer[PIPE_BUF];
	rapidjson::Reader         reader;
	rapidjson::FileReadStream frs;
	std::string_view          last_key;

	bounding_box box;
	state_t      state = state_t::before_json;
	bool         _good = true;

	std::function<bool(const bounding_box& box)> query_function;

	int inserted_scalars;
	void reset_box();
	void add_latitude(double latitude);
	void add_longitude(double longitude);

public:
	geojson_parser(FILE* file, std::function<bool(const bounding_box& box)> query_function);

	bool good() const;

	// Json handlers
	bool StartObject();
	bool EndObject(rapidjson::SizeType);
	bool StartArray();
	bool EndArray(rapidjson::SizeType);
	bool Key(const char* str, rapidjson::SizeType size, bool);
	bool String(const char* str, rapidjson::SizeType, bool);
	bool Double(double d);

	bool Default();
};

};
