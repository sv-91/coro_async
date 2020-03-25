cmake_minimum_required( VERSION 3.4.0 )

project(coroutines)

set(CMAKE_CXX_STANDARD 20)

find_package(Threads)

include_directories("/opt/local/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines -g")

add_subdirectory(src)
