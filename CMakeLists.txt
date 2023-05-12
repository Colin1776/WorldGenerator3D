cmake_minimum_required(VERSION 3.23.2)
project(WorldGenerator3D)

set(CMAKE_CXX_STANDARD 17)

add_executable(WorldGenerator3D src/main.cpp)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory("lib/glfw-3.3.8")
add_subdirectory("lib/glm")

target_link_libraries(WorldGenerator3D glfw)
target_link_libraries(WorldGenerator3D glm)

include_directories(incl)
