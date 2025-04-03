cmake_minimum_required(VERSION 3.30)

project(
  aurora-scicalc
  VERSION 0.0
  DESCRIPTION "Symbolic computation environment."
  LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

# file(COPY ${PROJECT_SOURCE_DIR}/scripts/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)
# file(GLOB HEAD_FILES ${PROJECT_SOURCE_DIR}/src/*.h)

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic -g -march=native)
endif()

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_executable(${PROJECT_NAME} ${SRC_FILES})


#target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/src/)
target_link_libraries(${PROJECT_NAME} m)
