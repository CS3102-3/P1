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

#include <cassert>

#include "r_tree.hpp"

utec::r_tree::r_tree():
	root(nullptr)
{};

utec::r_tree::node::node(const bounding_box& box):
	leaf(true),
	size(0),
	box(box)
{};

void utec::r_tree::_search(node* n, const bounding_box& box, std::vector<coordinate>& v)
{
	if(n == nullptr || !n->box.overlaps(box))
		return;

	if(n->leaf)
	{
		for(size_t i = 0; i < n->size; i++)
		{
			if(n->points[i].in(box))
				v.push_back(n->points[i]);
		}
	}
	else
	{
		for(size_t i = 0; i < n->size; i++)
		{
			_search(n->children[i], box, v);
		}
	}
}

void utec::r_tree::insert(const coordinate& coord)
{
	// TODO
}

std::vector<utec::coordinate> utec::r_tree::search(const bounding_box& box)
{
	std::vector<coordinate> v;

	_search(root, box, v);

	return v;
}

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
	os.write((char*)r_tree::N, sizeof(r_tree::N));

	if(rt.root)
		os << *rt.root;

	return os;
}

std::istream& utec::operator>>(std::istream& is, r_tree::node& n)
{
	// TODO

	return is;
}

std::ostream& utec::operator<<(std::ostream& os, const r_tree::node& n)
{
	os.write((char*)&n.leaf, sizeof(n.leaf));
	os.write((char*)&n.size, sizeof(n.size));
	os.write((char*)&n.box, sizeof(n.box));

	if(n.leaf)
	{
		os.write((char*)n.points, sizeof(n.points[0])*n.size);
	}
	else
	{
		for(size_t i = 0; i < n.size; i++)
		{
			assert(n.children[i]);
			os << *n.children[i];
		}
	}

	return os;
}
