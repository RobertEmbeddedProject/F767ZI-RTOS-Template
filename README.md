# PROJECT_NAME

STM32F767ZI 

## Hardware
- Board: NUCLEO-F767ZI


## Make sure ARM Compiler is installed, as well as STM32Cube Core extension
## Also install CMAKE if not already from CMAKE website
Open terminal and find ARM toolchain (usually comes with cubeIDE):
Get-ChildItem "C:\ST" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue
Get-ChildItem "C:\Program Files" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue
Get-ChildItem "C:\Program Files (x86)" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue    

then run:

###              $env:Path += ";YourFilePathHereKeepSemicolon"
###              arm-none-eabi-gcc --version

## Build and Flash
do NOT use cmake -B build -S .

First time run:
cmake --preset stm32-debug
cmake --build --preset stm32-debug

cmake --build build
or, CTRL+SHFT+B to Build
Fn+F5 to Flash


