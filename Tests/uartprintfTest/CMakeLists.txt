
project(uartprintfTest)


set(EXECUTABLE ${PROJECT_NAME})

add_executable(${EXECUTABLE} 
    main.cpp
)

target_link_libraries(${EXECUTABLE} PUBLIC 
    FibrationLib
)

target_compile_definitions(${EXECUTABLE} PRIVATE
    -DUSE_HAL_DRIVER
    -DSTM32F303xC
)

target_compile_options(${EXECUTABLE} PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    -specs=nosys.specs
    -specs=nano.specs
    -Wl,--gc-sections
    -static

    -fdata-sections
    -ffunction-sections

    -Wall

    $<$<CONFIG:Debug>:-Og>
    $<$<CONFIG:Release>:-O2>
)
# TODO something with these flags, move them to hardware layer maybe
target_link_options(${EXECUTABLE} PRIVATE
    -T${CMAKE_SOURCE_DIR}/Fibration/Components/Parts/Hardware/STM32F303CCTX_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard

    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
    -specs=nosys.specs
    -specs=nano.specs
    -static
    -Wl,--gc-sections

    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--print-memory-usage
)

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-size ${EXECUTABLE}
)

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
)
