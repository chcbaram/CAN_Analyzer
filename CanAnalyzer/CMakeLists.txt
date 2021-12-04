cmake_minimum_required(VERSION 3.12)

set(PRJ_NAME CanAnalyzer)



# Specify the cross compiler.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_C_STANDARD    11)
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_C_COMPILER    arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER  arm-none-eabi-g++)
SET(CMAKE_LINKER        arm-none-eabi-ld)
SET(CMAKE_NM            arm-none-eabi-nm)
SET(CMAKE_OBJCOPY       arm-none-eabi-objcopy)
SET(CMAKE_OBJDUMP       arm-none-eabi-objdump)
SET(CMAKE_RANLIB        arm-none-eabi-ranlib)

# Look for includes and libraries only in the target system prefix.
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Disable compiler checks.
set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)

project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)


set(EXECUTABLE ${PRJ_NAME}.elf)


file(GLOB SRC_FILES CONFIGURE_DEPENDS
  src/bsp/*.c
  src/bsp/FreeRTOS/*.c
  src/bsp/FreeRTOS/Source/*.c
  src/bsp/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/*.c
  src/bsp/FreeRTOS/Source/CMSIS_RTOS/*.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
  src/ap/*.cpp
  src/bsp/cmsis/*.c
  src/common/*.c
  src/hw/*.c
  src/lib/STM32H7xx_HAL_Driver/Src/*.c
  src/lib/littlefs/*.c
)

add_executable(${EXECUTABLE} 
  src/main.cpp
  src/bsp/startup/startup_stm32h743vitx.s

  src/bsp/FreeRTOS/Source/portable/MemMang/heap_4.c
  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)



target_include_directories(${EXECUTABLE} PRIVATE 
  src  
  src/ap
  src/bsp
  src/common 
  src/common/core
  src/common/hw/include 
  src/hw
  src/lib

  src/bsp/FreeRTOS
  src/bsp/FreeRTOS/Source/CMSIS_RTOS
  src/bsp/FreeRTOS/Source/include
  src/bsp/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
  src/hw/core/usb
  src/hw/core/usb/core
  src/hw/core/usb/usb_cdc
  src/bsp/cmsis/device/Include
  src/bsp/cmsis/Include
  src/lib/STM32H7xx_HAL_Driver/Inc
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32H743xx 
  -D_GLIBCXX_INCLUDE_NEXT_C_HEADERS
  -DLFS_THREADSAFE
  )

target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-d16 
  -mfloat-abi=hard

  -fdata-sections
  -ffunction-sections

  -Wall

  -O0
  )

target_link_options(${EXECUTABLE} PRIVATE
  -T../src/bsp/ldscript/STM32H743VITX_FLASH.ld
  -mcpu=cortex-m7
  -mthumb
  -mfpu=fpv5-d16 
  -mfloat-abi=hard
  -specs=nano.specs
  -lc
  -lm
  -lstdc++
  -lsupc++
  -lnosys
  -Wl,-Map=${PRJ_NAME}.map,--cref
  -Wl,--gc-sections
  )


set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND arm-none-eabi-size --format=berkeley --totals "$<TARGET_FILE:${EXECUTABLE}>"
        COMMENT "Invoking: Cross ARM GNU Print Size")