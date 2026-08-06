cmake_minimum_required(VERSION 3.20)

project(main)

add_executable(main main.cpp)

find_package(glfw)
find_package(glad)

add_subdirectory(gengine)

target_link_libraries(main gengine glad glfw)
