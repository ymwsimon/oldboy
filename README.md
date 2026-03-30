# oldboy — Game Boy Emulator

oldboy is a Game Boy (DMG) emulator written in C using SDL 3 for graphic, user input handling and audio. This repository contains the emulator core, a Makefile-based build, and references used during development.

## Features
- CPU emulation (Sharp LR35902 instruction set)
- Memory map and MBC (cartridge) support
- Basic PPU (pixel rendering) and frame timing
- Input via keyboard (via SDL 3)
- Audio output via SDL 3
- Save game
- Build via Makefile
- Test ROM compatibility support for debugging

## Requirements
- C compiler (gcc or clang)
- make
- SDL3 library
- SDL3 development headers

On Debian(trixie or later):
- sudo apt install build-essential libsdl3-dev libsdl3-0

For more info about SDL3:
- [SDL3](https://wiki.libsdl.org/SDL3/FrontPage)

## Build
Build using the provided Makefile:
- From the project root:
  - make
- Clean the build:
  - make clean

The produced binary will be named `oldboy` unless your Makefile specifies otherwise.

## Usage
Run the emulator with a Game Boy ROM:
- ./oldboy path-to-rom.gb

Example:
- ./oldboy roms/Tetris.gb

Control:
- Up: W
- Down: S
- Left: A
- Right: D
- Button A: K
- Button B: J
- Select: Z
- Start: X

To quit:
- Esc

Demo:
![Demo](https://github.com/ymwsimon/oldboy/blob/main/resources/tetris.gif?raw=true)

## Project structure
- src        — C source files
- include    — headers
- resources/rom — rom
- oldboy-testcases — test rom
- Makefile    — build instructions

## Testing
- Use public gb-test-roms and PPU/CPU test ROMs to validate behavior:
  - https://github.com/retrio/gb-test-roms
  - dmg-acid2 PPU tests: https://github.com/mattcurrie/dmg-acid2
  - https://github.com/Gekkio/mooneye-test-suite

## Technical reference (Game Boy hardware)
- CPU: Sharp LR35902 (hybrid of Z80 and Intel 8080-like), ~4.194304 MHz (DMG)
- Display: 160×144 pixels, 59.7 Hz, 20 × 18 tilemap (160×144)
- VRAM: 8 KB
- Internal RAM: 8 KB (plus switchable banks with MBC)
- Cartridge ROM: up to 8 MB with bank switching (MBC1/3/5 etc.)
- Audio: 4 channels (2 square waves, 1 waveform, 1 noise)
- Input: 4 directions, A, B, Start, Select

## References / Sources
- SDL: https://www.libsdl.org/
- Game Boy Pandocs: https://gbdev.io/pandocs/
- From-zero implementation guide (Chinese): https://zhuanlan.zhihu.com/p/676908347
- Gameboy Development Manual V1.1: https://archive.org/details/GameBoyProgManVer1.1/mode/2up
- Background info (Copetti): https://www.copetti.org/writings/consoles/game-boy/
- Test ROMs: https://github.com/retrio/gb-test-roms/tree/master
- Opcode tables: https://gbdev.io/gb-opcodes/optables/ and https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
- CPU manual: http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
- Technical reference: https://gekkio.fi/files/gb-docs/gbctr.pdf
- PPU testing ROM: https://github.com/mattcurrie/dmg-acid2
