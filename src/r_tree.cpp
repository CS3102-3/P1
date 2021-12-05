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

#include "r_tree.hpp"

utec::r_tree::r_tree():
	root(nullptr)
{};

utec::r_tree::node::node(bounding_box box):
	box(box),
	leaf(true),
	size(0)
{};

utec::r_tree::node::~node()
{
	if(!leaf)
	{
		for(size_t i = 0; i < size; i++)
		{
			delete children[i];
		}
	}
}

utec::r_tree::~r_tree()
{
	delete(root);
}

std::istream& utec::operator>>(std::istream& is, r_tree& rt)
{
	// TODO

	return is;
}

std::ostream& utec::operator<<(std::ostream& os, const r_tree& rt)
{
	// TODO

	return os;
}
