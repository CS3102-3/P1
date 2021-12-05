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

#include "geo_utils.hpp"

bool utec::coordinate::in(const bounding_box& box)
{
	return latitude  >= box.min_c.latitude
		&& longitude >= box.min_c.longitude
		&& latitude  <= box.max_c.latitude
		&& longitude <= box.max_c.longitude
	;
}

bool utec::bounding_box::overlaps(const bounding_box& box)
{
	bool in_lat =
		(
			min_c.latitude >= box.min_c.latitude &&
			min_c.latitude <= box.max_c.latitude
		) ||
		(
			max_c.latitude >= box.min_c.latitude &&
			max_c.latitude <= box.max_c.latitude
		)
	;

	bool in_lon =
		(
			min_c.longitude >= box.min_c.longitude &&
			min_c.longitude <= box.max_c.longitude
		) ||
		(
			max_c.longitude >= box.min_c.longitude &&
			max_c.longitude <= box.max_c.longitude
		)
	;

	return in_lat && in_lon;
}
