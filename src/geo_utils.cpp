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

#include "geo_utils.hpp"

bool utec::coordinate::in(const bounding_box &box) const {
    return latitude >= box.min_c.latitude
           && longitude >= box.min_c.longitude
           && latitude <= box.max_c.latitude
           && longitude <= box.max_c.longitude;
}

utec::bounding_box utec::bounding_box::merge(const utec::bounding_box &b0, const utec::bounding_box &b1) {
    coordinate urp(std::max(b0.max_c.latitude, b1.max_c.latitude), std::max(b0.max_c.longitude, b1.max_c.longitude));
    coordinate llp(std::min(b0.min_c.latitude, b1.min_c.latitude), std::min(b0.min_c.longitude, b1.min_c.longitude));
    return {llp, urp};
}

utec::bounding_box::bounding_box(const std::vector<coordinate> &coordinates) {
    double minLat{}, maxLat{}, minLon{}, maxLon{};
    for (const auto &c: coordinates) {
        minLat = std::min(minLat, c.latitude);
        maxLat = std::max(maxLat, c.latitude);
        minLon = std::min(minLon, c.longitude);
        maxLon = std::max(maxLon, c.longitude);
    }
    min_c = {minLat, minLon};
    max_c = {maxLat, maxLon};
}

utec::bounding_box::bounding_box(const std::vector<utec::r_tree::node *> &nodes) {
    double minLat{}, maxLat{}, minLon{}, maxLon{};
    for (const auto &c: nodes) {
        minLat = std::min(minLat, c->box.latitude);
        maxLat = std::max(maxLat, c->box.latitude);
        minLon = std::min(minLon, c->box.longitude);
        maxLon = std::max(maxLon, c->box.longitude);
    }
    min_c = {minLat, minLon};
    max_c = {maxLat, maxLon};
}

bool utec::bounding_box::overlaps(const bounding_box &box) const {
    bool in_lat =
            (
                    min_c.latitude >= box.min_c.latitude &&
                    min_c.latitude <= box.max_c.latitude
            ) ||
            (
                    max_c.latitude >= box.min_c.latitude &&
                    max_c.latitude <= box.max_c.latitude
            );

    bool in_lon =
            (
                    min_c.longitude >= box.min_c.longitude &&
                    min_c.longitude <= box.max_c.longitude
            ) ||
            (
                    max_c.longitude >= box.min_c.longitude &&
                    max_c.longitude <= box.max_c.longitude
            );

    return in_lat && in_lon;
}

double utec::bounding_box::area() const {
    return (max_c.latitude - min_c.latitude) * (max_c.longitude - min_c.longitude);
}

void utec::bounding_box::add(const utec::bounding_box &box) {
    max_c = {std::max(max_c.latitude, box.max_c.latitude), std::max(max_c.longitude, box.max_c.longitude)};
    min_c = {std::min(min_c.latitude, box.min_c.latitude), std::min(min_c.longitude, box.min_c.longitude)};
}

void utec::bounding_box::add(utec::coordinate coor) {
    max_c = {std::max(max_c.latitude, coor.latitude), std::max(max_c.longitude, coor.longitude)};
    min_c = {std::min(max_c.latitude, coor.latitude), std::min(max_c.longitude, coor.longitude)};
}

double utec::bounding_box::hypotheticalArea(const utec::bounding_box &box) const {
    coordinate ma_c = {std::max(max_c.latitude, box.max_c.latitude), std::max(max_c.longitude, box.max_c.longitude)};
    coordinate mi_c = {std::min(min_c.latitude, box.min_c.latitude), std::min(min_c.longitude, box.min_c.longitude)};
    return (ma_c.latitude - mi_c.latitude) * (ma_c.longitude - mi_c.longitude);
}

double utec::bounding_box::hypotheticalArea(utec::coordinate coor) const {
    coordinate ma_c = {std::max(max_c.latitude, coor.latitude), std::max(max_c.longitude, coor.longitude)};
    coordinate mi_c = {std::min(min_c.latitude, coor.latitude), std::min(min_c.longitude, coor.longitude)};
    return (ma_c.latitude - mi_c.latitude) * (ma_c.longitude - mi_c.longitude);
}
