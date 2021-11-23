# Proyeto 1
# Copyright © 2021 grupo 3
#
# p1 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# p1 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with p1.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.13.0)

project(p1
	VERSION 0.0.0
	DESCRIPTION "Proyecto 1"
	HOMEPAGE_URL "https://github.com/CS3102-3/P1"
)

# Without this, paths are not relative in the sources list
cmake_policy(SET CMP0076 NEW)

Include(FetchContent)
include(CPack)
include(FindPkgConfig)
include(GNUInstallDirs)

# The program itself
add_executable(${PROJECT_NAME})

# C++ version
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 17
)

# Packages
find_package(bash-completion)

add_subdirectory(src) # Sources list
add_subdirectory(pkg) # Packaging

## Shared libraries
#pkg_check_modules(shared
#	REQUIRED IMPORTED_TARGET
#		libfoo1
#		libfoo2
#)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.0.0-preview3
)

FetchContent_MakeAvailable(Catch2)


#add_executable(tests test.cpp)
#target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

# Header only libraries
pkg_check_modules(header_only
	REQUIRED IMPORTED_TARGET
		#catch2
		RapidJSON
)

# Linking
target_link_libraries(${PROJECT_NAME}
#	PRIVATE
#		PkgConfig::shared

	INTERFACE
		PkgConfig::header_only
)

# Default flags
if(UNIX)
	if(NOT (DEFINED ENV{CFLAGS} OR CMAKE_C_FLAGS))
		set(CMAKE_C_FLAGS "-Wall -Wextra -g -Og")
	endif()
	if(NOT (DEFINED ENV{CXXFLAGS} OR CMAKE_CXX_FLAGS))
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -g -Og")
	endif()
endif()

# Bash completion
if(BASH_COMPLETION_FOUND)
	file(WRITE ${CMAKE_BINARY_DIR}/bash-completion/${PROJECT_NAME}
		"complete -F _longopt ${PROJECT_NAME}"
	)

	install(FILES
		${CMAKE_BINARY_DIR}/bash-completion/${PROJECT_NAME}
		DESTINATION ${BASH_COMPLETION_COMPLETIONSDIR}/
	)
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)
