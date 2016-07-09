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

# add the executable
add_executable (demo demo.cpp)

# ncframe
include_directories ("${NCF_INCLUDES}")

# ncurses
find_package (Curses REQUIRED)
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
  add_executable(unittest unittest.cpp)

  # Standard linking to gtest stuff.
  target_link_libraries(unittest gtest gtest_main)

  # Extra linking for the project.
  target_link_libraries(unittest ${CURSES_LIBRARIES})

  # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  add_test(do_unit_test unittest)
endif()
