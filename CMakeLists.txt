cmake_minimum_required(VERSION 3.30)
set(CMAKE_C_COMPILER "gcc")

project(
  aurora-scicalc
  VERSION 0.0
  DESCRIPTION "Symbolic computation environment."
  LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_library(READLINE readline REQUIRED)

file(COPY ${PROJECT_SOURCE_DIR}/scripts/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/scripts/")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
# file(GLOB HEAD_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

# add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -march=native)

set(SHARED_FLAGS "-lncurses -lreadline -std=gnu11 -Wall -Wextra -Wpedantic -Werror -g -march=native")
set(CMAKE_C_FLAGS_DEBUG_INIT SHARED_FLAGS)
set(CMAKE_C_FLAGS_RELEASE_INIT SHARED_FLAGS "-DNDEBUG -O3")

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SRC_FILES})


#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(${PROJECT_NAME} m ${READLINE})
