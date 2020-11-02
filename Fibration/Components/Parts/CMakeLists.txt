cmake_minimum_required(VERSION 3.13)

project(Parts)

add_library(Parts STATIC 
#   button.cpp
#   button.hpp
    knob.cpp
    knob.hpp
    led.cpp
    led.hpp
    rotary.cpp
    rotary.hpp
    sigIn.cpp
    sigIn.hpp
    sigOut.cpp
    sigOut.hpp
    slider.cpp
    slider.hpp
    )

add_subdirectory(Hardware)
include_directories(Hardware)

target_link_libraries(Parts PUBLIC Hardware)
