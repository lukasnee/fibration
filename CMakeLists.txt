cmake_minimum_required(VERSION 3.13)

project(fibration)

add_subdirectory(modules)
# add_subdirectory(tests)
# NOTE: cmake only supports one toolchain for one build. Unit test use toolchain 
# for native build (gcc for x86-64) therefore must be built seperately.
