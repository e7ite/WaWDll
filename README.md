# WaWDll
Simple mod menu for Call of Duty World at War on Steam for Windows. Reverse engineering this game has taught me so much about the x86 instruction set, layouts of data in the executable, the layout of Windows PE executables, etc. Using this knowledge I was able to find many game objects, and I learned a ton of how the Call of Duty engine works. This game has no ASLR so we are allowed to get static pointers to game objects in the .data segment, and game subroutines in the .text segment in the executable. I used Explorer Suite to dump the game, and I used IDA to reverse engineer the game and find all the information that I have in the project.

If you have any questions about this program or want any explanations to anything, feel free to message me on Discord @E7ite#1156

## Features
- Mod menu
- Support for All Resolutions
- Regular Aimbot for Zombies
- ESP (Extra Sensory Perception) for Zombies.
- No Recoil
- FOV
- Super Steady Aim
- God Mode
- No Clip
- No Target
- Infinite Ammo
- No Flinch

## Future Updates
- Host All-Client Mod Menu
- Rendering the game mouse cursor always when menu is open

## Controls
- Home button opens the menu
- Backspace exits submenus and menus
- Use mouse cursor to highlight options, and left-click to select.

## Build Instructions
1. Download and install Microsoft Detours Research package https://github.com/microsoft/Detours
2. Build the DLL in x86 mode (the game is 32-bit) using Microsoft Visual Studio (Tested on 2017 and 2019)
3. Launch the game and inject the DLL with your favorite DLL injector.

## Preview
![](/screenshot.png)
