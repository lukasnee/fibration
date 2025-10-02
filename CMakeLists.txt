cmake_minimum_required(VERSION 4.0)

add_compile_options(-Ofast)

project(fibration LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -Werror)

include(system/config/config.cmake) # TODO move config into project folders?

add_subdirectory(extern/ln)

add_subdirectory(system)

add_subdirectory(modules)
# add_subdirectory(tests) NOTE: cmake only supports one toolchain for one build.
# Unit test use toolchain for native build (gcc for x86-64) therefore must be
# built seperately.
