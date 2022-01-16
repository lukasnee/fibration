cmake_minimum_required(VERSION 3.5)

project(unitTests)

ADD_EXECUTABLE(${PROJECT_NAME}
    source/main.cpp
    # tests/.templateTests.cpp
    tests/demo.cpp
)

include_directories(
    include
)
