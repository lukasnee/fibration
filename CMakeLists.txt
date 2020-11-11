cmake_minimum_required(VERSION 3.13)

project(Fibration)

enable_language(CXX C ASM)

# set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(Tests)
#add_subdirectory(Modules)

add_subdirectory(Fibration)