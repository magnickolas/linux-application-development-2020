cmake_minimum_required(VERSION 3.17)
project(PCREShow VERSION 0.1)
add_executable(pcreshow main.c pcre.c wstring.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -O2 -std=c99")

set(
  CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/"
)

set(CURSES_NEED_WIDE true)
find_package(Curses REQUIRED)
find_package(PCRE2 REQUIRED)

include_directories(
  ${CURSES_INCLUDE_DIRS}
  ${PCRE2_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(pcreshow ${CURSES_LIBRARIES} ${PCRE2_LIBRARIES})

if(without_UTF)
  add_definitions(-DNO_UTF)
endif()

if(UNIX)
  add_custom_target(allclean @echo cleaning for source distribution)
  set(
    DISTCLEANED
    pcreshow
    cmake.depends
    cmake.check_depends
    CMakeCache.txt
    CMakeFiles
    cmake.check_cache
    *.cmake
    Makefile
    core core.*
    gmon.out
    *~
  )

  add_custom_command(
    DEPENDS clean
    COMMENT "distribution clean"
    COMMAND rm
    ARGS -Rf CMakeTmp ${DISTCLEANED}
    TARGET allclean
  )
endif(UNIX)
