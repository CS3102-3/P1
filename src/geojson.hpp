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

#include <iterator>

#include <rapidjson/filereadstream.h>

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

class iterative_parser
{
private:
	FILE* file;

public:
	class iterator
	{
	private:
		enum class state_t
		{
			before_json,
			after_json
		};

		bounding_box box;
		state_t state;
		iterative_parser& ip;

		friend class iterative_parser;

		void get_next_box();

	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = bounding_box;
		using pointer           = value_type*;
		using reference         = value_type&;

		iterator(iterative_parser& ip);

		reference operator*()
		{
			return box;
		}

		pointer operator->()
		{
			return &box;
		}

		iterator& operator++()
		{
			get_next_box();
			return *this;
		}

		friend bool operator==(const iterator& l, const iterator& r)
		{
			return l.state == r.state;
		}

		friend bool operator!=(const iterator& l, const iterator& r)
		{
			return !(l == r);
		}
	};

	iterator begin()
	{
		return iterator(*this);
	}

	iterator end()
	{
		auto it = iterator(*this);
		it.state = iterator::state_t::after_json;

		return it;
	}

	iterative_parser(FILE* file);
};

};
