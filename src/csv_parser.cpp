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

#include <charconv>
#include <iostream>
#include <sstream>

#include "csv_parser.hpp"

bool utec::csv_parser::iterator::try_next_file(const std::string& csv_path)
{
	csv_ifs.open(csv_path);

	if(!csv_ifs.is_open())
	{
		perror(csv_path.c_str());
		return false;
	}

	return check_header();
}

bool utec::csv_parser::iterator::check_header()
{
	std::string line;

	if(!std::getline(csv_ifs, line))
		return false;

	std::istringstream ss(line);

	size_t i = 0;
	bool longitude = false, latitude = false;;
	for(std::string token; std::getline(ss, token, ','); i++)
	{
		if(token == "Pickup_longitude")
		{
			longitude_i = i;
			longitude = true;
		}
		else if(token == "Pickup_latitude")
		{
			latitude_i = i;
			latitude = true;
		}
	}

	return longitude && latitude;
}

bool utec::csv_parser::iterator::read_csv_line()
{
	std::string line;

	if(!std::getline(csv_ifs, line))
		return false;

	std::istringstream ss(line);

	size_t i = 0;
	for(std::string token; std::getline(ss, token, ','); i++)
	{
		if(i == longitude_i)
		{
			if(!parse_token(last_coordinate.longitude, token))
				return false;
		}
		else if(i == latitude_i)
		{
			if(!parse_token(last_coordinate.latitude, token))
				return false;
		}
	}

	return true;
}

bool utec::csv_parser::iterator::parse_token(
	double& value,
	const std::string& token
)
{
	auto [p, ec] = std::from_chars(
		token.data(),
		token.data() + token.size(),
		value
	);

	if(ec != std::errc())
	{
		std::cerr
			<< token
			<< ": "
			<< std::make_error_code(ec).message()
			<< '\n'
		;
		return false;
	}

	return true;
}

void utec::csv_parser::iterator::get_next_tuple()
{
	while(true)
	{
		// The csvs have already been iterated.
		if(csv_index >= parent.csv_paths.size())
			return;

		switch(state)
		{
			case state_t::before_csv:
				if(try_next_file(parent.csv_paths[csv_index]))
					state = state_t::before_tuples;
				else
					csv_index++;

				continue;

			case state_t::before_tuples:
				if(!read_csv_line())
				{
					if(csv_ifs.eof())
					{
						csv_ifs.close();
						state = state_t::before_csv;
						csv_index++;
					}
					continue;
				}
				break;
		}

		return;
	}
}
