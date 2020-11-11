cmake_minimum_required(VERSION 3.13)

project(FibrationLib)
	
add_library(FibrationLib OBJECT 
	# TODO
	# fibration.cpp
	fibration.hpp
	)

add_subdirectory(Components)

target_link_libraries(FibrationLib PUBLIC Components)
