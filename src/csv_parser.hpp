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

#include <span>
#include <string>
#include <fstream>

#include "geo_utils.hpp"

namespace utec
{

class csv_parser
{
private:
	std::span<std::string> csv_paths;

public:
	class iterator
	{
	private:
		enum class state_t
		{
			before_csv,
			before_tuples,
		};

		csv_parser& parent;
		state_t     state;
		size_t      csv_index;
		coordinate  last_coordinate;

		std::ifstream csv_ifs;
		size_t        latitude_i;
		size_t        longitude_i;

		bool try_next_file(const std::string& csv_path);
		bool check_header();
		bool read_csv_line();
		bool parse_token(double& value, const std::string& token);
		void get_next_tuple();

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = coordinate;
		using pointer           = value_type*;
		using reference         = value_type&;

		iterator(csv_parser& parent, bool end):
			parent(parent),
			state(state_t::before_csv),
			csv_index(end ? parent.csv_paths.size() : 0)
		{
			get_next_tuple();
		};

		reference operator*()
		{
			return last_coordinate;
		}

		pointer operator->()
		{
			return &last_coordinate;
		}

		iterator& operator++()
		{
			get_next_tuple();
			return *this;
		}

		friend bool operator==(const iterator& l, const iterator& r)
		{
			return l.csv_index == r.csv_index && l.state == r.state;
		}

		friend bool operator!=(const iterator& l, const iterator& r)
		{
			return !(l == r);
		}
	};

	iterator begin()
	{
		return {*this, false};
	};

	iterator end()
	{
		return {*this, true};
	}

	csv_parser(std::span<std::string> csv_paths):
		csv_paths(csv_paths)
	{};
};

};
