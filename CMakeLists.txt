cmake_minimum_required (VERSION 2.8)

project (hello)

set (CMAKE_CXX_STANDARD 11)

# add the executable
add_executable (hello hello.cpp)

# ncframe
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/ncframe")

# ncurses
find_package (Curses REQUIRED)
#set (CURSES_INCLUDE_DIR "/usr/local/include/ncursesw")
#set (CURSES_LIBRARIES "/usr/local/lib/libncursesw.dylib")
#set (CURSES_INCLUDE_DIR "/usr/local/opt/ncurses/include")
#set (CURSES_LIBRARIES "/usr/local/opt/ncurses/lib/libncursesw.dylib")
#if (APPLE)
#    set (CURSES_INCLUDE_DIR "/usr/local/opt/ncurses/include")
#    set (CURSES_LIBRARIES "/usr/local/opt/ncurses/lib/libncursesw.dylib")
#endif ()
#if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#    set (CURSES_INCLUDE_DIR "/usr/include/ncursesw")
#    set (CURSES_LIBRARIES "/usr/lib/x86_64-linux-gnu/libncursesw.so")
#endif ()
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries (hello ${CURSES_LIBRARIES})
# ncurses end
