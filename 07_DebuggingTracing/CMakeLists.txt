cmake_minimum_required(VERSION 3.17)
project(rm_preload VERSION 0.1)
add_library(
  rm_preload SHARED
  rm_preload.c
)
target_link_libraries(rm_preload ${CMAKE_DL_LIBS})
enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)
