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

#include <iostream>

#include <rapidjson/error/en.h>

#include "geojson.hpp"

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
	return _good;
}

bool utec::geojson_parser::StartObject()
{
	// TODO
	std::cerr << "StartObject\n";
	return true;
}

bool utec::geojson_parser::EndObject(rapidjson::SizeType)
{
	// TODO
	std::cerr << "EndObject\n";
	return true;
}

bool utec::geojson_parser::StartArray()
{
	// TODO
	std::cerr << "StartArray\n";
	return true;
}

bool utec::geojson_parser::EndArray(rapidjson::SizeType)
{
	// TODO
	std::cerr << "EndArray\n";
	return true;
}

bool utec::geojson_parser::Key(const char* str, rapidjson::SizeType, bool)
{
	// TODO
	std::cerr << "Key\n";
	return true;
}

bool utec::geojson_parser::String(const char* str, rapidjson::SizeType, bool)
{
	// TODO
	std::cerr << "String\n";
	return true;
}

bool utec::geojson_parser::Double(double d)
{
	// TODO
	std::cerr << "Double\n";
	return true;
}

bool utec::geojson_parser::Default()
{
	return false;
}
