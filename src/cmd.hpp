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

#include <vector>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace utec
{

class cmd
{
private:
	enum class action_type
	{
		index,
		search
	};

	int argc;
	char** argv;

	action_type action;
	std::vector<std::string> csv_paths;
	std::string query_path;

	[[ noreturn ]]
	void usage(int exit_code) const;

	int index();
	int search();

	/// The path of the directory where the r-tree will be saved.
	static fs::path data_path;

	/// The path of the r-tree.
	static fs::path r_tree_path;

public:
	cmd(int argc, char** argv):
		argc(argc),
		argv(argv)
	{};

	int run();
};

};
