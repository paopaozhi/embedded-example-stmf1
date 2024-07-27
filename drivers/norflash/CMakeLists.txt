project(norflash)

add_library(${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME} PRIVATE
    USE_HAL_DRIVER
    STM32F103xB
)

target_sources(${PROJECT_NAME} PRIVATE
    norflash.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${BOARD_PATH}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ${BOARD_PATH}/Drivers/STM32F1xx_HAL_Driver/Inc
    ${BOARD_PATH}/Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ${BOARD_PATH}/Drivers/CMSIS/Include
    ${BOARD_PATH}/Core/Inc
)

target_include_directories(${PROJECT_NAME} INTERFACE
    .
)
