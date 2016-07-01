cmake_minimum_required (VERSION 2.8)

project (hello)

set (CMAKE_CXX_STANDARD 11)

# all header files
set (headers 
    hello.h
    )
# all src files
set (sources
    hello.cpp
    )

# add the executable
add_executable (hello ${sources} ${headers})

# add ncframe library
include_directories (../)
target_link_libraries (ncframe ../build/ncframe.a)
