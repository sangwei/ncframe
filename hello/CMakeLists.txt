cmake_minimum_required (VERSION 2.8)

project (hello)

set (CMAKE_CXX_STANDARD 11)

# all header files
set (headers
    )

# all src files
set (sources
    hello.cpp
    )

# add the executable
add_executable (hello ${sources} ${headers})

# libncframe
include_directories ("/Users/sangwei/Codes/lab/ncframe/ncframe")

# ncurses
#find_package (Curses REQUIRED)
#set (CURSES_INCLUDE_DIR "/usr/local/include/ncursesw")
#set (CURSES_LIBRARIES "/usr/local/lib/libncursesw.dylib")
#set (CURSES_INCLUDE_DIR "/usr/local/opt/ncurses/include")
#set (CURSES_LIBRARIES "/usr/local/opt/ncurses/lib/libncursesw.dylib")
if (APPLE)
    set (CURSES_INCLUDE_DIR "/usr/local/opt/ncurses/include")
    set (CURSES_LIBRARIES "/usr/local/opt/ncurses/lib/libncursesw.dylib")
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set (CURSES_INCLUDE_DIR "/usr/include/ncursesw")
    set (CURSES_LIBRARIES "/usr/lib/x86_64-linux-gnu/libncursesw.so")
endif ()
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries (hello ${CURSES_LIBRARIES})
# ncurses end
