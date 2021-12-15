// Proyecto 1
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

#include <algorithm>
#include "r_tree.hpp"

namespace utec {

    struct bounding_box;

    struct coordinate {
        double latitude{};
        double longitude{};

        coordinate() = default;

        coordinate(double lat, double lon) : latitude(lat), longitude(lon) {}

        bool in(const bounding_box &box) const;
    };

    struct bounding_box {
        coordinate min_c{};
        coordinate max_c{};

        static bounding_box merge(const bounding_box &, const bounding_box &);

        bounding_box() = default;

        explicit bounding_box(const coordinate &coord) : min_c(coord), max_c(coord) {}

        bounding_box(std::vector<r_tree::node *> b) : min_c(b.min_c), max_c(b.max_c) {}

        bounding_box(coordinate ll, coordinate ur) : min_c(ll), max_c(ur) {}

        explicit bounding_box(const std::vector<coordinate> &coordinates);

        explicit bounding_box(const std::vector<utec::r_tree::node*> &nodes);

        bool overlaps(const bounding_box &box) const;

        double area() const;

        double hypotheticalArea(const bounding_box &box) const;

        double hypotheticalArea(coordinate coor) const;

        void add(const bounding_box &box);

        void add(coordinate coor);

        bounding_box merge(const coordinate &c0, const coordinate &c1);
    };
}
