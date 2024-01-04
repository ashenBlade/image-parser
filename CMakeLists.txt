cmake_minimum_required(VERSION 3.22)

project(image-parser VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 14)

FILE(GLOB_RECURSE sources "src/*.cpp" "src/*.c")

message(STATUS "sources ${sources}")

add_executable(${PROJECT_NAME} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC include)


