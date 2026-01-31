project(
  higgs
  LANGUAGES
  VERSION 0.0.1)

ln_add_firmware(higgs ${CMAKE_SOURCE_DIR}/system/msp/STM32F303RETX_FLASH.ld
                ${PROJECT_SOURCE_DIR}/openocd.cfg)
target_link_libraries(higgs fibsys)
target_sources(higgs PRIVATE main.cpp)
target_compile_options(higgs PRIVATE -fsingle-precision-constant)
target_link_libraries(higgs ln::logger)
