cmake_minimum_required (VERSION 2.8)

# Options. Turn on with 'cmake -Dtest=ON'.
option (test "Build all tests." OFF) # Makes boolean 'test' available.

# Make PROJECT_SOURCE_DIR, PROJECT_BINARY_DIR, and PROJECT_NAME available.
set (PROJECT_NAME ncframe)
project (${PROJECT_NAME})

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS "-g -Wall")

# If you want your own include/ directory, set this, and then you can do
# include_directories(${COMMON_INCLUDES}) in other CMakeLists.txt files.
# set(COMMON_INCLUDES ${PROJECT_SOURCE_DIR}/include)
set (NCF_INCLUDES ${PROJECT_SOURCE_DIR}/ncframe)
set (NCF_DEMO_DIR ${PROJECT_SOURCE_DIR}/demo)
set (NCF_UNIT_DIR ${PROJECT_SOURCE_DIR}/unittest)

# add the demo executable
add_executable (demo ${NCF_DEMO_DIR}/demo.cpp)

# ncframe
include_directories ("${NCF_INCLUDES}")

# ncurses
set (CURSES_INCLUDE_DIR "na")
set (CURSES_LIBRARIES "na")
if (APPLE)
    if(EXISTS "/usr/lib/libncursesw.dylib")
        set (CURSES_INCLUDE_DIR "/usr/include/ncursesw/include")
        set (CURSES_LIBRARIES "/usr/lib/libncursesw.dylib")
    else()
        if(EXISTS "/usr/local/opt/ncurses/lib/libncursesw.dylib")
            set (CURSES_INCLUDE_DIR "/usr/local/opt/ncurses/include")
            set (CURSES_LIBRARIES "/usr/local/opt/ncurses/lib/libncursesw.dylib")
        else()
            if(EXISTS "/usr/local/lib/libncursesw.dylib")
                set (CURSES_INCLUDE_DIR "/usr/local/include/ncursesw/include")
                set (CURSES_LIBRARIES "/usr/local/lib/libncursesw.dylib")
            endif()
        endif()
    endif()
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(EXISTS "/usr/lib/x86_64-linux-gnu/libncursesw.so")
        set (CURSES_INCLUDE_DIR "/usr/include/ncursesw")
        set (CURSES_LIBRARIES "/usr/lib/x86_64-linux-gnu/libncursesw.so")
    endif()
endif ()
if ((${CURSES_INCLUDE_DIR} EQUAL "na") OR (${CURSES_LIBRARIES} EQUAL "na"))
    # use default find, probably no widechar support
    find_package (Curses REQUIRED)
endif()
include_directories (${CURSES_INCLUDE_DIR})
target_link_libraries (demo ${CURSES_LIBRARIES})

################################
# Testing
################################
if (test)
  # This adds another subdirectory, which has 'project(gtest)'.
  add_subdirectory(lib/googletest)

  enable_testing()

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories (${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

  ##############
  # Unit Tests
  ##############
  add_executable(unittest ${NCF_UNIT_DIR}/unittest.cpp)

  # Standard linking to gtest stuff.
  target_link_libraries(unittest gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(unittest ${CURSES_LIBRARIES})

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(do_unit_test unittest)
endif()
