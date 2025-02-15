cmake_minimum_required(VERSION 3.22)  

include("cmake/gcc-arm-none-eabi.cmake")

# Define all projects
project(loader_copy_united_proj)
enable_language(C CXX ASM)
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/.venv")
    message(FATAL_ERROR "Virtual environment not found. Please create it first:\n"
                       "python -m venv .venv\n"
                       "source .venv/bin/activate\n"
                       "pip install -r requirements.txt")
endif()
find_program(VENV_PYTHON
    NAMES python3 python
    PATHS "${CMAKE_SOURCE_DIR}/.venv/bin"
    NO_DEFAULT_PATH
    REQUIRED
)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Common settings for all projects
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# MCU settings
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F407xx)

# Define startup files
set(BOOT_STARTUP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/boot/startup/startup_stm32f407vgtx.s")
set(LOADER_STARTUP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/loader/startup/startup_stm32f407vgtx.s")
set(UPDATER_STARTUP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/updater/startup/startup_stm32f407vgtx.s")
set(APP_STARTUP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/app/startup/startup_stm32f407vgtx.s")

# Common CPU parameters
set(cpu_PARAMS 
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# Common compiler flags for all targets
set(COMMON_FLAGS
    ${cpu_PARAMS}
    -Wall
    -Wextra
    -Wno-unused-parameter
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -fno-builtin
    $<$<CONFIG:Debug>:-O0 -g3 -ggdb>
    $<$<CONFIG:Release>:-Os -g0>
)

# Common linker flags for all targets
set(COMMON_LINKER_FLAGS
    ${cpu_PARAMS}
    -Wl,-gc-sections
    -Wl,--print-memory-usage
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8
)

# Common include directories
set(COMMON_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
)

# Common source files for all targets
set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f4xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/stm32f4xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
)

# Create executables for each component
add_executable(boot_debug)
add_executable(loader_debug)
add_executable(updater_debug)
add_executable(app_debug)

# Function to configure common settings for each target
function(configure_target target)
    message("Configuring target: ${target}")
    target_compile_options(${target} PRIVATE ${COMMON_FLAGS})
    target_include_directories(${target} PRIVATE ${COMMON_INCLUDES})
    
    # Base definitions for all targets
    target_compile_definitions(${target} PRIVATE
        "DEBUG"
        "USE_HAL_DRIVER"
        ${MCU_MODEL}
    )

    # Target-specific definitions
    if(${target} STREQUAL "boot_debug")
        target_compile_definitions(${target} PRIVATE "P_BOOT")
    elseif(${target} STREQUAL "loader_debug")
        target_compile_definitions(${target} PRIVATE "P_LOADER")
    elseif(${target} STREQUAL "updater_debug")
        target_compile_definitions(${target} PRIVATE 
            "P_UPDATER"
            "MBEDTLS_CONFIG_FILE=<mbedtls_config.h>"
        )
    elseif(${target} STREQUAL "app_debug")
        target_compile_definitions(${target} PRIVATE "P_APP")
    endif()

    target_link_options(${target} PRIVATE ${COMMON_LINKER_FLAGS})
endfunction()

# Configure all targets
configure_target(boot_debug)
configure_target(loader_debug)
configure_target(updater_debug)
configure_target(app_debug)

# Set specific includes for each target
target_include_directories(boot_debug PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/boot/inc)
target_include_directories(loader_debug PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/loader/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/nanoprintf
)
target_include_directories(updater_debug PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/nanoprintf
    ${CMAKE_CURRENT_SOURCE_DIR}/MBEDTLS/App
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/Middlewares/Third_Party/mbedTLS/include/mbedtls
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/Middlewares/Third_Party/mbedTLS/include
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/Middlewares/Third_Party/JANPATCH
)
target_include_directories(app_debug PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/app/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/nanoprintf
)

# Set linker scripts and generate map files
set_target_properties(boot_debug PROPERTIES LINK_FLAGS 
    "-T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F407VGTX_FLASH_BOOT.ld -Wl,-Map=${CMAKE_BINARY_DIR}/boot_debug.map")
set_target_properties(loader_debug PROPERTIES LINK_FLAGS 
    "-T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F407VGTX_FLASH_LOADER.ld -Wl,-Map=${CMAKE_BINARY_DIR}/loader_debug.map")
set_target_properties(updater_debug PROPERTIES LINK_FLAGS 
    "-T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F407VGTX_FLASH_UPDATER.ld -Wl,-Map=${CMAKE_BINARY_DIR}/updater_debug.map")
set_target_properties(app_debug PROPERTIES LINK_FLAGS 
    "-T${CMAKE_CURRENT_SOURCE_DIR}/linker/STM32F407VGTX_FLASH_APP.ld -Wl,-Map=${CMAKE_BINARY_DIR}/app_debug.map")

# Add sources to targets
target_sources(boot_debug PRIVATE
    ${BOOT_STARTUP_FILE}
    ${COMMON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/boot/src/system_stm32f4xx.c
)

target_sources(loader_debug PRIVATE
    ${LOADER_STARTUP_FILE}
    ${COMMON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/loader/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/loader/src/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/ring_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/uart_log.c
)

file(GLOB_RECURSE MBEDTLS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/updater/Middlewares/Third_Party/mbedTLS/library/*.c"
)

file(GLOB_RECURSE JANPATCH_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/updater/Middlewares/Third_Party/JANPATCH/*.c"
)

target_sources(updater_debug PRIVATE
    ${UPDATER_STARTUP_FILE}
    ${COMMON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/src/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/src/updater_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/updater/src/xmodem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/ring_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/uart_log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/MBEDTLS/App/mbedtls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
    ${MBEDTLS_SOURCES}
    ${JANPATCH_SOURCES}
)

target_sources(app_debug PRIVATE
    ${APP_STARTUP_FILE}
    ${COMMON_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/app/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/app/src/system_stm32f4xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/ring_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/misc/uart_log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/syscalls.c
)

# Post-build commands for all targets
foreach(TARGET boot_debug loader_debug updater_debug app_debug)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${TARGET}> ${TARGET}.bin
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET}>
    )
endforeach()

# Add memory analysis for each component
foreach(TARGET boot_debug loader_debug updater_debug app_debug)
    add_custom_command(TARGET ${TARGET} POST_BUILD
        COMMAND echo "Memory analysis for ${TARGET}:"
        COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/memap.py 
            -t GCC_ARM 
            ${CMAKE_BINARY_DIR}/${TARGET}.map
            -d3
        VERBATIM
    )
endforeach()

# Patch headers and merge binaries
add_custom_target(patch_and_merge ALL
    DEPENDS boot_debug loader_debug updater_debug app_debug
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/patch_image_header.py boot_debug.bin --skip-header
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/patch_image_header.py loader_debug.bin
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/patch_image_header.py updater_debug.bin
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/patch_image_header.py app_debug.bin
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/merge_images.py 
        boot_debug.bin loader_debug.bin updater_debug.bin app_debug.bin
    COMMAND cp merged_firmware.bin final_firmware.bin
    COMMENT "Processing binaries and creating final firmware"
)

# Flash targets
add_custom_target(flash_boot
    DEPENDS boot_debug
    COMMAND STM32_Programmer.sh -c port=swd mode=UR -d boot_debug.bin 0x08000000 -v -hardRst
)

add_custom_target(flash_full
    DEPENDS patch_and_merge
    COMMAND STM32_Programmer.sh -c port=swd mode=UR -d merged_firmware.bin 0x08000000 -v -hardRst
)

# Target for memory analysis only
add_custom_target(memory_analysis
    COMMAND echo "=== Memory analysis for all components ==="
    COMMAND echo "\\nBootloader memory usage:"
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/memap.py -t GCC_ARM ${CMAKE_BINARY_DIR}/boot_debug.map -d3
    COMMAND echo "\\nLoader memory usage:"
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/memap.py -t GCC_ARM ${CMAKE_BINARY_DIR}/loader_debug.map -d3
    COMMAND echo "\\nUpdater memory usage:"
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/memap.py -t GCC_ARM ${CMAKE_BINARY_DIR}/updater_debug.map -d3
    COMMAND echo "\\nApplication memory usage:"
    COMMAND ${VENV_PYTHON} ${CMAKE_SOURCE_DIR}/scripts/memap.py -t GCC_ARM ${CMAKE_BINARY_DIR}/app_debug.map -d3
    DEPENDS boot_debug loader_debug updater_debug app_debug
    COMMENT "Analyzing memory usage for all components"
    VERBATIM
)

get_target_property(UPDATER_COMPILE_FLAGS updater_debug COMPILE_OPTIONS)
message("UPDATER_COMPILE_FLAGS: ${UPDATER_COMPILE_FLAGS}")

get_target_property(UPDATER_COMPILE_DEFS updater_debug COMPILE_DEFINITIONS)
message("UPDATER_COMPILE_DEFS: ${UPDATER_COMPILE_DEFS}")

get_target_property(UPDATER_INCLUDE_DIRS updater_debug INCLUDE_DIRECTORIES) 
message("UPDATER_INCLUDE_DIRS: ${UPDATER_INCLUDE_DIRS}")