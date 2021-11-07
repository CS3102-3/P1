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

#include <cstdlib>
#include <getopt.h>
#include <iostream>

#include "cmd.hpp"

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
				std::cout << "Index TODO\n";
				break;

			case 's':
				std::cout << "Search " << optarg << " TODO\n";
				break;

			case '?':
				return EXIT_FAILURE;

			default:
				usage(EXIT_FAILURE);
		}
	}

	for(int i = optind; i < argc; i++)
	{
		std::cout << argv[i] << '\n';
	}

	return EXIT_SUCCESS;
}
