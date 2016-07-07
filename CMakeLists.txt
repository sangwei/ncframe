cmake_minimum_required (VERSION 2.8)

project (test)

set (CMAKE_CXX_STANDARD 11)

# add the executable
add_executable (test main.cpp)

# ncframe
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/ncframe")

# ncurses
find_package (Curses REQUIRED)
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries (test ${CURSES_LIBRARIES})
