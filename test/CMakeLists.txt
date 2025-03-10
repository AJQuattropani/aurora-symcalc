cmake_minimum_required(VERSION 3.30)

project(
  aurora-scicalc-tests
  VERSION 0.0
  DESCRIPTION "Basic unit tests on operations."
  LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

set(TESTED_SOURCE_DIR ${PROJECT_SOURCE_DIR}/../src/)

file(GLOB SRC_FILES ${TESTED_SOURCE_DIR}*.c)

list(FILTER SRC_FILES EXCLUDE REGEX ".*main\\.c$")
#list(APPEND TEST_FILES ${PROJECT_SOURCE_DIR}/test.c)

message("Collecting these sources: ${SRC_FILES} \n")

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -g -march=native)
endif()

file(GLOB TEST_FILES ${PROJECT_SOURCE_DIR}/*.c)
message("Collecting these tests: ${TEST_FILES} \n")

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

foreach(test ${TEST_FILES})
  get_filename_component( testname ${test} NAME_WE)
  message("Building executable for ${testname}")
  add_executable(${testname} ${test} ${SRC_FILES})
  target_include_directories(${testname} PRIVATE ${TESTED_SOURCE_DIR})
  target_link_libraries(${testname} m)
endforeach(test ${TEST_FILES})

