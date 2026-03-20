# guitar_pedal

STM32F767ZI + ILI9341 + LVGL

## Hardware
- Board: NUCLEO-F767ZI
- Display: 3.2" ILI9341 SPI TFT
- Interface: SPI1, write-only

## LCD wiring
- PA5  -> SCK
- PA7  -> SDI / MOSI
- PD14 -> CS
- PD15 -> DC
- PF12 -> RESET
- 3V3  -> VCC / LED
- GND  -> GND

## Make sure ARM Compiler is installed
Open terminal and find ARM toolchain (usually comes with cubeIDE):
Get-ChildItem "C:\ST" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue
Get-ChildItem "C:\Program Files" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue
Get-ChildItem "C:\Program Files (x86)" -Recurse -Filter arm-none-eabi-gcc.exe -ErrorAction SilentlyContinue    

then run:

#              $env:Path += ";YourFilePathHereKeepSemicolon"
#              arm-none-eabi-gcc --version

## Build
cmake --build build


