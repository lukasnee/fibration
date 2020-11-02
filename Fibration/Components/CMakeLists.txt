cmake_minimum_required(VERSION 3.13)

project(Components)

add_library(Components STATIC 
    nco.cpp
    nco.hpp
    )

add_subdirectory(Parts)

target_link_libraries(Components PUBLIC Parts)
