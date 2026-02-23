from pathlib import Path

base_dir = Path.cwd()

# Files in the top directory
root_files = [
    "CMakeLists.txt",
    "toolchain-arm-none-eabi.cmake",
    "linker.ld"
]

# Folder structure
structure = {
    "src": ["main.c"],
    "drivers": ["exti.c", "exti.h", "gpio.c", "gpio.h", "syscfg.c", "syscfg.h", "uart.c", "uart.h"],
    "core": ["faults.c","nvic.c", "nvic.h", "runtime.c", "system.c", "system.h"],
    "startup": ["startup.s", "faults.s"],
    "include" : ["device.h"],
    "isr" : ["isr.c"]
}

# Create root files
for file in root_files:
    file_path = base_dir / file
    file_path.touch(exist_ok=True)

# Create folders and their files
for folder, files in structure.items():
    folder_path = base_dir / folder
    folder_path.mkdir(parents=True, exist_ok=True)

    for file in files:
        (folder_path / file).touch(exist_ok=True)

print("Project structure created successfully!")