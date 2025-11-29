project(
  higgs
  LANGUAGES
  VERSION 0.0.1)

add_executable(${PROJECT_NAME})

fibsys_firmware()

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR} # for version.def # TODO: remove version.def,
                                   # use CMake config instead
)
target_compile_options(${PROJECT_NAME} PRIVATE -fsingle-precision-constant)

target_link_options(${PROJECT_NAME} PRIVATE
                    -T${CMAKE_SOURCE_DIR}/system/msp/STM32F303RETX_FLASH.ld)

target_link_libraries(${PROJECT_NAME} fibsys ln::logger)
