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

utec::r_tree::r_tree() :
        root(nullptr) {}

utec::r_tree::node::node() :
        leaf(true),
        size(0) {}

utec::r_tree::node::node(const bounding_box &box) :
        leaf(true),
        size(0),
        box(box) {}

utec::r_tree::node::node(const std::vector<coordinate> &coordinates) : leaf(true) {
    if (coordinates.size() > N) throw std::invalid_argument("Node cannot possessed more than N coordinates.");
    size = coordinates.size();
    std::copy(coordinates.begin(), coordinates.end(), points);
    box = bounding_box(coordinates);
}

utec::r_tree::node::node(const std::vector<node *> &childrens) : leaf(false) {
    if (childrens.size() > N) throw std::invalid_argument("Node cannot possessed more than N childrens.");
    size = childrens.size();
    std::copy(childrens.begin(), childrens.end(), points);
    box = bounding_box(childrens);
}

void utec::r_tree::node::updatePoints(const std::vector<coordinate> &coordinates) {
    if (leaf) {
        size = 0;
        box = bounding_box(points[0]);
        for (const auto &p: coordinates) {
            big_enough(p);
            points[size++] = p;
        }
    }
}

void utec::r_tree::node::updateChildrens(const std::vector<node *> &childrens) {
    if (!leaf) {
        size = 0;
        box = bounding_box(childrens[0]->box);
        for (const auto &c: childrens) {
            big_enough(c->box);
            children[size++] = c;
        }
    }
}

void utec::r_tree::node::big_enough(const coordinate &coord) {
    box.min_c.latitude = std::min(box.min_c.latitude, coord.latitude);
    box.min_c.longitude = std::min(box.min_c.longitude, coord.longitude);
    box.max_c.latitude = std::max(box.max_c.latitude, coord.latitude);
    box.max_c.longitude = std::max(box.max_c.longitude, coord.longitude);
}

void utec::r_tree::node::big_enough(bounding_box &box) {
    box.min_c.latitude = std::min(box.min_c.latitude, box.min_c.latitude);
    box.min_c.longitude = std::min(box.min_c.longitude, box.min_c.longitude);
    box.max_c.latitude = std::max(box.max_c.latitude, box.max_c.latitude);
    box.max_c.longitude = std::max(box.max_c.longitude, box.max_c.longitude);
}

void utec::r_tree::_search(node *n, const bounding_box &box, std::vector<coordinate> &v) const {
    if (n == nullptr || !n->box.overlaps(box))
        return;

    if (n->leaf) {
        for (size_t i = 0; i < n->size; i++) {
            if (n->points[i].in(box))
                v.push_back(n->points[i]);
        }
    } else {
        for (size_t i = 0; i < n->size; i++) {
            _search(n->children[i], box, v);
        }
    }
}

template<typename T>
T utec::r_tree::pickNext(const std::vector<T> &values, std::unordered_set<size_t> &set, const bounding_box &b0,
                         const bounding_box &b1) {
    double maxDiff = -1;
    T result{};
    for (const auto &i: set) {
        double d0 =
                bounding_box::merge(bounding_box(values[i]), b0).area() - bounding_box(values[i]).area() - b0.area();
        double d1 =
                bounding_box::merge(bounding_box(values[i]), b1).area() - bounding_box(values[i]).area() - b1.area();
        double diff = std::abs(d0 - d1);
        if (diff > maxDiff) {
            maxDiff = diff;
            result = values[i];
        }
    }
    return result;
}

template<typename T>
std::pair<T, T>
utec::r_tree::pickSeeds(const std::vector<T> &values, std::unordered_set<size_t> &set) {
    size_t ip0{}, ip1{};
    double maxArea = -1;
    for (size_t i = 0; i < values.size(); ++i)
        for (size_t j = i + 1; j < values.size(); ++j) {
            auto b = bounding_box::merge(bounding_box(values[i]), bounding_box(values[j]));
            auto d = b.area() - bounding_box(values[i]).area() - bounding_box(values[j]).area();
            if (d > maxArea) {
                maxArea = d;
                ip0 = i;
                ip1 = j;
            }
        }
    std::pair<T, T> result(values[ip0], values[ip1]);
    set.erase(ip0);
    set.erase(ip1);
    return result;
}

template<typename T>
std::pair<std::vector<T>, std::vector<T>> utec::r_tree::quadraticSplit(const std::vector<T> &values) {
    std::unordered_set<size_t> set{};
    for (size_t i = 0; i < values.size(); ++i) set.insert(i);

    std::pair<std::vector<T>, std::vector<T>> result{};
    auto seeds = pickSeeds(values, set);
    result.first.push_back(seeds.first);
    auto group0 = utec::bounding_box(seeds.first);
    result.second.push_back(seeds.second);
    auto group1 = utec::bounding_box(seeds.second);

    while (!set.empty()) {
        auto seed = pickNext(values, set, group0, group1);
        auto a0 = group0.hypotheticalArea(seed);
        auto a1 = group1.hypotheticalArea(seed);
        if (a0 < a1) {
            group0.add(seed);
            result.first.push_back(seed);
        } else {
            group1.add(seed);
            result.second.push_back(seed);
        }
    }
    return result;
}

utec::r_tree::node *utec::r_tree::_insert(coordinate &coord, r_tree::node *n) {
    node *additionalNode = nullptr;
    if (n->leaf) {
        if (n->size < N) {
            n->big_enough(coord);
            n->points[root->size++] = coord;
            return nullptr;
        } else {
            std::vector<coordinate> set;
            std::copy(n->points, n->points + N, set);
            set.push_back(coord);
            auto s = quadraticSplit(set);
            n->updatePoints(s.first);
            return new node(s.second);
        }
    } else {
        bool flag = true;
        for (size_t i = 0; i < n->size; ++i) {
            if (coord.in(n->children[i]->box)) {
                additionalNode = _insert(coord, n->children[i]);
                flag = false;
            }
        }
        if (flag) {
            size_t best = 0;
            for (size_t i = 0; i < n->size; ++i) {
                if (n->children[i]->box.hypotheticalArea(coord) < n->children[best]->box.hypotheticalArea(coord))
                    best = i;
            }
            n->children[best]->big_enough(coord);
            additionalNode = _insert(coord, n->children[best]);
        }
    }
    if (additionalNode != nullptr) {
        if (n->size < N) {
            n->children[n->size++] = additionalNode;
            return nullptr;
        } else {
            std::vector<node *> bbs;
            std::copy(n->children, n->children + N, bbs);
            bbs.push_back(additionalNode);
            auto s = quadraticSplit(bbs);
            n->updateChildrens(s.first);
            return new node(s.second);
        }
    }
    return nullptr;
}


void utec::r_tree::insert(const coordinate &coord) {
    if (root == nullptr) {
        root = new node(bounding_box(coord));
        root->points[root->size++] = coord;
    } else {
        // TODO
    }
}

std::vector<utec::coordinate> utec::r_tree::search(const bounding_box &box) const {
    std::vector<coordinate> v;

    _search(root, box, v);

    return v;
}

utec::r_tree::node::~node() {
    if (!leaf) {
        for (size_t i = 0; i < size; i++) {
            delete children[i];
        }
    }
}

utec::r_tree::~r_tree() {
    delete (root);
}

std::istream &utec::operator>>(std::istream &is, r_tree &rt) {
    size_t N;
    is.read((char *) &N, sizeof(N));

    assert(N == r_tree::N);

    if (!is.eof()) {
        delete rt.root;
        is >> *(rt.root = new r_tree::node);
    }

    return is;
}

std::ostream &utec::operator<<(std::ostream &os, const r_tree &rt) {
    os.write((char *) r_tree::N, sizeof(r_tree::N));

    if (rt.root)
        os << *rt.root;

    return os;
}

std::istream &utec::operator>>(std::istream &is, r_tree::node &n) {
    is.read((char *) &n.leaf, sizeof(n.leaf));
    is.read((char *) &n.size, sizeof(n.size));
    is.read((char *) &n.box, sizeof(n.box));

    if (n.leaf) {
        is.read((char *) n.points, sizeof(n.points[0]) * n.size);
    } else {
        for (size_t i = 0; i < n.size; i++) {
            assert(!is.eof());
            is >> *(n.children[i] = new r_tree::node);
        }
    }

    return is;
}

std::ostream &utec::operator<<(std::ostream &os, const r_tree::node &n) {
    os.write((char *) &n.leaf, sizeof(n.leaf));
    os.write((char *) &n.size, sizeof(n.size));
    os.write((char *) &n.box, sizeof(n.box));

    if (n.leaf) {
        os.write((char *) n.points, sizeof(n.points[0]) * n.size);
    } else {
        for (size_t i = 0; i < n.size; i++) {
            assert(n.children[i]);
            os << *n.children[i];
        }
    }

    return os;
}

