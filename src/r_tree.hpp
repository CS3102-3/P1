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

#include <functional>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "geo_utils.hpp"

namespace utec {

    class r_tree {
    private:
        static const size_t N = 10;

        struct node {
            bool leaf;
            size_t size;
            bounding_box box;

            union {
                // Leaf node
                coordinate points[N]{};

                // Internal node
                node *children[N];
            };

            node();

            explicit node(const bounding_box &box);

            explicit node(const std::vector<coordinate> &coordinates);

            explicit node(const std::vector<node*> &childrens);

            ~node();

            void updatePoints(const std::vector<coordinate> &);

            void updateChildrens(const std::vector<node*> &);

            void big_enough(const coordinate &coord);

            void big_enough(bounding_box &box);
        };

        node *root;

        void _search(node *n, const bounding_box &box, std::vector<coordinate> &v) const;

        template<typename T>
        T pickNext(const std::vector<T> &values, std::unordered_set<size_t> &set, const bounding_box &,
                   const bounding_box &);

        template<typename T>
        std::pair<T, T> pickSeeds(const std::vector<T> &values, std::unordered_set<size_t> &set);

        template<typename T>
        std::pair<std::vector<T>, std::vector<T>> quadraticSplit(const std::vector<T> &);

        node *_insert(coordinate &coord, node *n);

    public:
        r_tree();

        ~r_tree();

        void insert(const coordinate &coord);

        std::vector<coordinate> search(const bounding_box &box) const;

        friend std::istream &operator>>(std::istream &is, r_tree &rt);

        friend std::ostream &operator<<(std::ostream &os, const r_tree &rt);

        friend std::istream &operator>>(std::istream &is, node &n);

        friend std::ostream &operator<<(std::ostream &os, const node &n);
    };

    std::istream &operator>>(std::istream &is, r_tree &rt);

    std::ostream &operator<<(std::ostream &os, const r_tree &rt);

    std::istream &operator>>(std::istream &is, r_tree::node &n);

    std::ostream &operator<<(std::ostream &os, const r_tree::node &n);

}
