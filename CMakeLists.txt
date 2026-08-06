cmake_minimum_required(VERSION 3.20)

project(main)

add_executable(main main.cpp)

find_package(glfw)
find_package(glad)

add_subdirectory(gengine)

target_include_directories(main PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cusInclude)

target_link_libraries(main gengine glad glfw)
