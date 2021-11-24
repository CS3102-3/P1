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

#include <climits>
#include <functional>
#include <iterator>

#include <rapidjson/filereadstream.h>
#include <rapidjson/reader.h>

#pragma once

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
		after_json,
		after_feature
	};

	char                      buffer[PIPE_BUF];
	rapidjson::Reader         reader;
	rapidjson::FileReadStream frs;

	bounding_box box;
	state_t state;
	bool _good = true;

	std::function<bool(const bounding_box& box)> query_function;

	void get_next_box();

public:
	geojson_parser(FILE* file, std::function<bool(const bounding_box& box)> query_function);

	bool good() const;

	// Json handlers
	bool Default();
};

};
