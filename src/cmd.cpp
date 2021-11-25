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

#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <wordexp.h>
#include <system_error>

#include "cmd.hpp"
#include "geojson.hpp"
#include "r_tree.hpp"

void utec::cmd::usage(int exit_code) const
{
	std::cout
		<< "Usage: p1 [-i CSV... | -sGEOJSON]\n"
		<< "\t-h, --help                 Show this help\n"
		<< "\t-i, --index CSV...         Index into the Rtree\n"
		<< "\t-s, --search=GEOJSON       Range search\n"
	;

	exit(exit_code);
}

int utec::cmd::run()
{
	if(argc < 2)
		usage(EXIT_FAILURE);

	int c;

	static const char shortopts[] = "his:";
	static const option options[] =
	{
		{"help",   no_argument,       nullptr, 'h'},
		{"index",  no_argument,       nullptr, 'i'},
		{"search", required_argument, nullptr, 's'},
		{nullptr,  0,                 nullptr, 0},
	};

	while((c = getopt_long(argc, argv, shortopts, options, nullptr)) != -1)
	{
		switch(c)
		{
			case 'h':
				usage(EXIT_SUCCESS);

			case 'i':
				action = action_type::index;
				break;

			case 's':
				action = action_type::search;
				query_path = optarg;
				break;

			case '?':
				return EXIT_FAILURE;

			default:
				usage(EXIT_FAILURE);
		}
	}

	for(int i = optind; i < argc; i++)
		csv_paths.push_back(argv[i]);

	if(!create_data_dir())
		return EXIT_FAILURE;

	switch(action)
	{
		case action_type::index:
			return index();

		case action_type::search:
			return search();
	}

	return EXIT_FAILURE;
}

int utec::cmd::index()
{
	// TODO

	return EXIT_FAILURE;
}

int utec::cmd::search()
{
	FILE* query = fopen(query_path.c_str(), "r");

	if(query == nullptr)
	{
		perror(query_path.c_str());
		return EXIT_FAILURE;
	}

	geojson_parser parser(query,
		[](const bounding_box& box) -> bool
		{
			// TODO
			std::cerr
				<< "min latitude: " << box.min_c.latitude << '\n'
				<< "min longitude: " << box.min_c.longitude << '\n'
				<< "max latitude: " << box.max_c.latitude << '\n'
				<< "max longitude: " << box.max_c.longitude << '\n'
			;

			return false;
		}
	);

	fclose(query);
	return parser.good()? EXIT_SUCCESS : EXIT_FAILURE;
}

const fs::path utec::cmd::data_path = []()
{
	wordexp_t p;

	int r = wordexp("\"${XDG_DATA_HOME:-$HOME/.local/share}/p1\"", &p, WRDE_NOCMD);
	assert(r == 0 && p.we_wordc == 1);

	fs::path path(p.we_wordv[0]);

	wordfree(&p);
	return path;
}();

const fs::path utec::cmd::r_tree_path = data_path / "r_tree";

bool utec::cmd::create_data_dir()
{
	std::error_code ec;

	fs::create_directories(data_path, ec);

	if(ec)
	{
		perror(data_path.string().c_str());
		return false;
	}

	return true;
}
