project(
  higgs
  LANGUAGES
  VERSION 0.0.1)

add_firmware(higgs ${CMAKE_SOURCE_DIR}/system/msp/STM32F303RETX_FLASH.ld)
target_sources(higgs PRIVATE main.cpp)
target_compile_options(higgs PRIVATE -fsingle-precision-constant)
target_link_libraries(higgs ln::logger)
