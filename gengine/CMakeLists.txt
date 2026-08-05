cmake_minimum_required(VERSION 3.20)

add_library(gengine ${CMAKE_CURRENT_SOURCE_DIR}/src/gengine.cpp)

target_include_directories(gengine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

