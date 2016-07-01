cmake_minimum_required (VERSION 2.8)

project (ncframe)

set (CMAKE_CXX_STANDARD 11)

# The version number.
set (ncf_VERSION_MAJOR 1)
set (ncf_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/ncfConfig.h.in"
  "${PROJECT_BINARY_DIR}/ncfConfig.h"
  )

# all header files
set(headers 
    ncf_win.h
    ncf_ctl.h
    )
# all src files
set(sources
    ncf_win.cpp
    ncf_ctl.cpp
    )

# add the binary tree to the search path for include files
# so that we will find ncfConfig.h
include_directories ("${PROJECT_BINARY_DIR}")

# add the library
add_library(ncframe ${sources} ${headers})

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
target_link_libraries (ncframe ${CURSES_LIBRARIES})
# ncurses end

# add the install targets
install (TARGETS ncframe DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/ncfConfig.h"        
         DESTINATION include)
