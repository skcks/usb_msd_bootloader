cmake_minimum_required(VERSION 2.8)

project(usb-bootloader C ASM)

file(GLOB CORE RELATIVE ${CMAKE_SOURCE_DIR} "CORE/*.c")
# file(GLOB FATFS RELATIVE ${CMAKE_SOURCE_DIR} 
#     "FATFS/src/*.c"
#     "FATFS/*.c"
# )
file(GLOB HARDWARE RELATIVE ${CMAKE_SOURCE_DIR}
    "HARDWARE/KEY/*.c"
    "HARDWARE/FLASH/*.c"
    "HARDWARE/LED/*.c"
    "HARDWARE/SPI/*.c"
    "HARDWARE/SD/*.c"
    "HARDWARE/*.c"
)
# file(GLOB MALLOC RELATIVE ${CMAKE_SOURCE_DIR} "MALLOC/*.c")
file(GLOB SPL RELATIVE ${CMAKE_SOURCE_DIR} "STM32F10x_FWLib/src/*.c")
file(GLOB SYSTEM RELATIVE ${CMAKE_SOURCE_DIR} 
    "SYSTEM/delay/*.c"
    "SYSTEM/sys/*.c"
    "SYSTEM/uart/*.c"
)
file(GLOB_RECURSE USB RELATIVE ${CMAKE_SOURCE_DIR} "USB/*.c")
file(GLOB USER RELATIVE ${CMAKE_SOURCE_DIR} "USER/*.c")

include_directories(
    CORE
    # FATFS
    # FATFS/src
    HARDWARE
    HARDWARE/KEY
    HARDWARE/FLASH
    HARDWARE/LED
    HARDWARE/SPI
    HARDWARE/SD
    MALLOC
    STM32F10x_FWLib/inc
    SYSTEM/delay
    SYSTEM/sys
    SYSTEM/usart
    USB/LIB
    USB/CONFIG
    USER
)

set(ELF_NAME ${PROJECT_NAME}.elf)
set(BIN_NAME ${PROJECT_NAME}.bin)
set(MAP_NAME ${PROJECT_NAME}.map)

set(LDSCRIPTS "-T STM32F103-MD.ld")
set(LDSCRIPTS_DIR "${PROJECT_SOURCE_DIR}/CORE")

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-g++)

# This two lines to avoid error 
# arm-none-eabi-g++: error: unrecognized command line option '-rdynamic'
# during compiler check. For some reasons test use '-rdynamic' by default, but arm-none-eabi-g* compilers does not support it
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

# To remove -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)

set(CMAKE_C_CXX_FLAGS "-mcpu=cortex-m3 -mthumb -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wall -Wextra -g3")
set(CMAKE_C_CXX_FLAGS_DEBUG   "-fno-move-loop-invariants -g -DDEBUG -Os")
set(CMAKE_C_CXX_FLAGS_RELEASE "-Os")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=gnu11")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_CXX_FLAGS} -std=gnu++11")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")
set(CMAKE_ASM_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}" )
set(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )

set(CMAKE_C_LINK_FLAGS)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDSCRIPTS} -Xlinker --gc-sections -L${LDSCRIPTS_DIR} -Wl,-Map,\"${MAP_NAME}\" --specs=nano.specs")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -nostartfiles")

add_definitions(-DSTM32F10X_MD)
add_definitions(-DUSE_STDPERIPH_DRIVER)



add_executable(${ELF_NAME} 
    CORE/startup_stm32f10x_md.s
    ${CORE}
    # ${FATFS}
    ${SPL}
    ${HARDWARE}
    ${MALLOC}
    ${SYSTEM}
    ${USB}
    ${USER}
)

add_custom_command(TARGET ${ELF_NAME}
                   POST_BUILD
                   COMMAND arm-none-eabi-size ${ELF_NAME}
                   )

add_custom_command(TARGET ${ELF_NAME}
                   POST_BUILD
                   COMMAND arm-none-eabi-objcopy ARGS -O binary ${ELF_NAME}  ${BIN_NAME}
                   )


