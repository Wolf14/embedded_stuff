set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_ROOT "G:/Software/compilers/gcc_arm/gcc-arm-none-eabi-10.3-2021.10")
set(TOOLCHAIN_BIN "${TOOLCHAIN_ROOT}/bin")

set(CMAKE_C_COMPILER    "${TOOLCHAIN_BIN}/arm-none-eabi-gcc.exe")
set(CMAKE_ASM_COMPILER    "${TOOLCHAIN_BIN}/arm-none-eabi-gcc.exe")
set(CMAKE_OBJCOPY    "${TOOLCHAIN_BIN}/arm-none-eabi-objcopy.exe")
set(CMAKE_SIZE       "${TOOLCHAIN_BIN}/arm-none-eabi-size.exe")

set(CMAKE_TRY_COMPILE_TARGET_TYPE           STATIC_LIBRARY)

set(COMMON_FLAGS        "-mcpu=cortex-m4 -mthumb -g -O0")

set(CMAKE_C_FLAGS       "${COMMON_FLAGS}")
set(CMAKE_ASM_FLAGS       "${COMMON_FLAGS}")