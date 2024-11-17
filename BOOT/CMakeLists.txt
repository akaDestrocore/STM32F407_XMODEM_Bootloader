cmake_minimum_required(VERSION 3.22)  

#
# User is free to modify the file as much as necessary
#

include("cmake/gcc-arm-none-eabi.cmake")

project(BOOT)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Specify the C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# MCU settings
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F407xx)
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/PROJECT)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/TEST)

# Define the startup file
set(STARTUP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Core/Startup/startup_stm32f407vgtx.s")

# Set compile options common to both configurations
set(cpu_PARAMS 
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# ===================================================
# Debug Configuration
# ===================================================

# Define the source files based on the configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    # Sources
    file(GLOB sources_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Src/*.c
        ${PROJECT_DIR}/*.c
        ${PROJECT_DIR}/misc/*.c
    )

    # Include directories for all compilers
    set(include_DIRS)

    # Include directories for each compiler

    # GNU C
    set(include_c_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
        ${PROJECT_DIR}
        ${PROJECT_DIR}/misc
    )

    # GNU C++
    set(include_cxx_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    )

    # Assembly
    set(include_asm_DIRS)

    # Symbols definition for all compilers
    set(symbols_SYMB
        "DEBUG"
        "USE_HAL_DRIVER"
        ${MCU_MODEL}
    )

    # Symbols definition for each compiler
    set(symbols_c_SYMB)
    set(symbols_cxx_SYMB)
    set(symbols_asm_SYMB)

    # Link directories and names of libraries
    set(link_DIRS

    )

    set(link_LIBS

    )

    # Linker script
    set(linker_script_SRC   ${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld)

    # Compiler options
    set(compiler_OPTS)

    # Linker options
    set(linker_OPTS)

    # Link directories setup
    # Must be before executable is added
    link_directories(${CMAKE_PROJECT_NAME}_debug ${link_DIRS})

    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME}_debug)

    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME}_debug PUBLIC ${STARTUP_FILE} ${sources_SRCS})

    # Add include paths
    target_include_directories(${CMAKE_PROJECT_NAME}_debug PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
    )

    # Add project symbols (macros)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_debug PRIVATE
        ${symbols_SYMB}
        $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
        $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
        $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

        # Configuration specific
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Release>: >
    )


    # Add linked libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}_debug ${link_LIBS})

    # Compiler options
    target_compile_options(${CMAKE_PROJECT_NAME}_debug PRIVATE
        ${cpu_PARAMS}
        ${compiler_OPTS}
        -Wall
        -Wextra
        -Wno-unused-parameter
        $<$<COMPILE_LANGUAGE:C>: >
        $<$<COMPILE_LANGUAGE:CXX>:

        # -Wno-volatile
        # -Wold-style-cast
        # -Wuseless-cast
        # -Wsuggest-override
        >
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
        $<$<CONFIG:Debug>:-Og -g3 -ggdb>
        $<$<CONFIG:Release>:-Og -g0>
    )

    # Linker options
    target_link_options(${CMAKE_PROJECT_NAME}_debug PRIVATE
        -T${linker_script_SRC}
        ${cpu_PARAMS}
        ${linker_OPTS}
        -Wl,-Map=${CMAKE_PROJECT_NAME}.map
        --specs=nosys.specs
        -Wl,--start-group
        -lc
        -lm
        -lstdc++
        -lsupc++
        -Wl,--end-group
        -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
        -Wl,--print-memory-usage
    )

    # Execute post-build to print size, generate hex and bin
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_debug POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}_debug>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}_debug> ${CMAKE_PROJECT_NAME}_debug.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}_debug> ${CMAKE_PROJECT_NAME}_debug.bin
    )

# ===================================================
# Test-Debug Configuration
# ===================================================
elseif(CMAKE_BUILD_TYPE STREQUAL "Test-Debug")

    # Sources
    file(GLOB sources_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Src/*.c
        ${PROJECT_DIR}/*.c
        ${PROJECT_DIR}/misc/*.c
        ${TEST_DIR}/*.cpp
    )

    # Exclude specific files
    list(REMOVE_ITEM sources_SRCS 
        "${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/main.c" 
    )

    # Print sources after removal
    message(STATUS "Sources after removal: ${sources_SRCS}")

    # Include directories for all compilers
    set(include_DIRS)

    # Include directories for each compiler

    # GNU C
    set(include_c_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
        ${PROJECT_DIR}
        ${PROJECT_DIR}/misc
    )

    # GNU C++
    set(include_cxx_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
        ${TEST_DIR}/libs
        ${TEST_DIR}
        ${PROJECT_DIR}
        ${PROJECT_DIR}/misc
    )

    # Assembly
    set(include_asm_DIRS)

    # Symbols definition for all compilers
    set(symbols_SYMB)

    # Symbols definition for each compiler
    set(symbols_c_SYMB
        "DEBUG"
        "USE_HAL_DRIVER"
        ${MCU_MODEL}
    )
    set(symbols_cxx_SYMB
        "DEBUG"
        "USE_HAL_DRIVER"
        ${MCU_MODEL}
        "CPPUTEST_STD_CPP_LIB_DISABLED"
        "CPPUTEST_STD_C_LIB_DISABLED"
        "UNIT_TEST"
    )
    set(symbols_asm_SYMB
        "DEBUG"
    )

    # Link directories and names of libraries
    set(link_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/TEST/libs/cpputest-3_8
    )

    set(link_LIBS
        cpputest-3_8
    )

    # Linker script
    set(linker_script_SRC   ${CMAKE_CURRENT_SOURCE_DIR}/STM32F407VGTX_FLASH.ld)

    # Compiler options
    set(compiler_OPTS)

    # Linker options
    set(linker_OPTS)

    # Link directories setup
    # Must be before executable is added
    link_directories(${CMAKE_PROJECT_NAME}_test ${link_DIRS})

    # Enable CTest
    enable_testing()

    # Create an executable object type
    add_executable(${CMAKE_PROJECT_NAME}_test)

    # Add sources to executable
    target_sources(${CMAKE_PROJECT_NAME}_test PUBLIC ${STARTUP_FILE} ${sources_SRCS})

    # Add include paths
    target_include_directories(${CMAKE_PROJECT_NAME}_test PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
    )

    # Add project symbols (macros)
    target_compile_definitions(${CMAKE_PROJECT_NAME}_test PRIVATE
        ${symbols_SYMB}
        $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
        $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
        $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

        # Configuration specific
        $<$<CONFIG:Debug>:DEBUG>
        $<$<CONFIG:Debug>:UNIT_TEST>
        $<$<CONFIG:Release>: >
    )


    # Add linked libraries
    target_link_libraries(${CMAKE_PROJECT_NAME}_test ${link_LIBS})

    # Compiler options
    target_compile_options(${CMAKE_PROJECT_NAME}_test PRIVATE
        ${cpu_PARAMS}
        ${compiler_OPTS}
        -Wall
        -Wextra
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -Wno-sign-compare
        $<$<COMPILE_LANGUAGE:C>: >
        $<$<COMPILE_LANGUAGE:CXX>:
        -Og 
        -g3 
        -ggdb
        # -Wno-volatile
        # -Wold-style-cast
        # -Wuseless-cast
        # -Wsuggest-override
       >
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
        # moved Debug specific flags to `COMPILE_LANGUAGE:CXX` section
        $<$<CONFIG:Release>:-Og -g0>
    )

    # Linker options
    target_link_options(${CMAKE_PROJECT_NAME}_test PRIVATE
        -T${linker_script_SRC}
        ${cpu_PARAMS}
        ${linker_OPTS}
       # -Wl,-Map=${CMAKE_PROJECT_NAME}.map
        --specs=nosys.specs
        -Wl,--start-group
        -lc
        -lm
        -lstdc++
        -lsupc++
        -Wl,--end-group
        -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
        -Wl,--print-memory-usage
    )

    # Execute post-build to print size, generate hex and bin
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME}_test POST_BUILD
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}_test>
        COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}_test> ${CMAKE_PROJECT_NAME}_test.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}_test> ${CMAKE_PROJECT_NAME}_test.bin
    )

    add_test(NAME ${CMAKE_PROJECT_NAME}_TEST COMMAND cmd /c "${CMAKE_SOURCE_DIR}/test.bat" "${CMAKE_SOURCE_DIR}" $<TARGET_FILE:${CMAKE_PROJECT_NAME}_test>)

endif()

