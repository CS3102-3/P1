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

namespace utec
{

class cmd
{
private:
	int argc;
	char** argv;

	[[ noreturn ]]
	void usage(int exit_code) const;

public:
	cmd(int argc, char** argv):
		argc(argc),
		argv(argv)
	{};

	int run();
};

};
