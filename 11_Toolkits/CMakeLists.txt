cmake_minimum_required(VERSION 3.17)
project(Count VERSION 0.1)
add_executable(count count.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -O0 -g -std=c99")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
target_include_directories(count PRIVATE ${GLIB_INCLUDE_DIRS})
target_link_libraries(count PRIVATE ${GLIB_LDFLAGS})
