cmake_minimum_required(VERSION 3.20)

project(stm32_bringup LANGUAGES C ASM)

add_executable(stm32.elf
    startup/startup.s
    startup/faults.s
    src/main.c
    core/runtime.c
    core/system.c
    core/faults.c
    core/nvic.c
    drivers/gpio.c
    drivers/uart.c
    drivers/syscfg.c
    drivers/exti.c
    drivers/timer.c
    drivers/timer.h
    isr/isr.c
)

target_include_directories(stm32.elf PRIVATE
    ${CMAKE_SOURCE_DIR}
)

target_compile_options(stm32.elf PRIVATE
        -ffreestanding
        -fno-builtin
)

target_link_options(stm32.elf PRIVATE
        -T${CMAKE_SOURCE_DIR}/linker.ld
        -nostartfiles
        -Wl,-Map=stm32.map
)

add_custom_command(TARGET stm32.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}    -O binary stm32.elf stm32.bin
    COMMENT "Generating stm32.bin"
)