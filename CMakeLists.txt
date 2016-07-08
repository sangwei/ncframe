cmake_minimum_required (VERSION 2.8)

project (demo)

set (CMAKE_CXX_STANDARD 11)

# add the executable
add_executable (demo main.cpp)

# ncframe
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/ncframe")

# ncurses
find_package (Curses REQUIRED)
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries (demo ${CURSES_LIBRARIES})
