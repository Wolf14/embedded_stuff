cmake_minimum_required(VERSION 3.20)

project(stm32_bringup LANGUAGES C ASM)

set(STM32CUBE_F4_ROOT "F:/projects/comp_sci/embedded/libraries/STM32CubeF4-master")
set(FREERTOS_KERNEL_ROOT "F:/projects/comp_sci/embedded/libraries/FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel")

add_executable(stm32.elf
    startup/startup.s
    startup/faults.s
    src/main.c
    core/runtime.c
    core/faults.c
    drivers/gpio.c
    drivers/timer.c
    drivers/dma.c
    drivers/uart.c
    motion/stepper.c
    rtos/app_tasks.c
    rtos/motion_task.c
    rtos/monitor_task.c
    rtos/console_task.c
    bsp/console.c
    isr/isr.c

)

target_sources(stm32.elf PRIVATE
    ${FREERTOS_KERNEL_ROOT}/tasks.c
    ${FREERTOS_KERNEL_ROOT}/queue.c
    ${FREERTOS_KERNEL_ROOT}/list.c
    ${FREERTOS_KERNEL_ROOT}/portable/GCC/ARM_CM4F/port.c
    ${FREERTOS_KERNEL_ROOT}/portable/MemMang/heap_4.c
)


target_include_directories(stm32.elf PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${STM32CUBE_F4_ROOT}/Drivers/CMSIS/Include
    ${STM32CUBE_F4_ROOT}/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ${FREERTOS_KERNEL_ROOT}/include
    ${FREERTOS_KERNEL_ROOT}/portable/GCC/ARM_CM4F
    ${CMAKE_SOURCE_DIR}/config
)

target_compile_definitions(stm32.elf PRIVATE
    STM32F446xx
)

target_compile_options(stm32.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -ffreestanding
    -fno-builtin
)

target_link_options(stm32.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -T${CMAKE_SOURCE_DIR}/linker.ld
    -nostartfiles
    -Wl,-Map=stm32.map
)

add_custom_command(TARGET stm32.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}    -O binary stm32.elf stm32.bin
    COMMENT "Generating stm32.bin"
)