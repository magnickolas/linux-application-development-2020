find_program(BASH_PROGRAM bash)
add_test(test1 ${BASH_PROGRAM} "${CMAKE_SOURCE_DIR}/tests/test1.sh")
add_test(test2 ${BASH_PROGRAM} "${CMAKE_SOURCE_DIR}/tests/test2.sh")
