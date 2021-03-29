# MC5000_DevKit

A development board that brings the [Shenzhen IO game](https://www.zachtronics.com/shenzhen-io/) to real hardware.

This documentation is intended for people who want to build/extend/bugfix the hardware. If you have a board and just want to play with it you'll find the user documentation [here](https://rickp.github.io/MC5000_DevKit/).


## Overview

The project consists of three parts. The firmware for the PFS173 microcontroller used in this project, a circuit board that connects to a computer via USB and a cross-platform desktop application that is used to programm the board.

The main use of this is to teach beginners the basics of programming and electronics by solving problems with increasing difficulty. Users have to make electric connection on the circuit board with jumper cable and programm the two microcontrollers with a simplified assembler language to solve the tasks.


## The circuit board

![Cuircuit board picture](pcb/docs/MC5000_Board-PCB.svg?raw=true "MC5000 DevKit")

The board is designed in KiCAD 5, an OpenSource electronics CAD package. In the spirit of the game it is made with the cheapest components I could find. 

The design files are contained in the subfolder 'pcb'. The production files that are needed to make completely assembled boards (Gerbers, Pick&Place, BOM) are compiled automatically and can be found on the [releases page](https://github.com/RickP/MC5000_DevKit/releases) in the zip package "PCB_fabrication_files_for_jlcpcb.zip". 

Currently everything optimized for the chinese board manufacturer [JLCPB](https://jlcpcb.com). Just upload the gerbers (2 layers, 1.6mm PCB), choose "SMT Assembly" and upload the BOM and Pick&Place file when asked for it. For some reason you can have green, black, red or blue boards if you order up to 30 boards. For 50 boards they have to be green or black. If you order 50 boards each one is under 7 Euros including shipping and taxes to the EU (with the 'EuroPackage' shipping option').

Please check if all components are mounted the right way around when ordering. I tried to make suer everything fits but I may not have ordered the lates iteration myself. Also it would be nice it you give me some feedback if you order these. E.g. mention me in a tweet about it (@r1ckp).


## Microcontroller Firmware

The board uses 4 Padauk PFS173 microcontrollers. These are super cheap chinese 8bit microcontrollers that cost about 8cent per piece. They have 3000 kilowords of flash ROM and 256 bytes of RAM.

The firmware for the two application controllers (a.k.a. MC5000) and the buzzer and display controller are in the subfolder 'firmware'. They are made with the [Free PDK toolchain](https://free-pdk.github.io) an open-source toolchain for many of the Padauk microcontrollers. 

In order to flash the ICs you have to make or buy an Easy PDK programmer. The programmer is open hardware. I got mine assempled from amazon but you can make one yourself [production files](https://kitspace.org/boards/github.com/free-pdk/easy-pdk-programmer-hardware/).

If you juast want to flash the firmware you need the programmer and the [easy-pdk-programmer software](https://github.com/free-pdk/easy-pdk-programmer-software). Firmware binaries can be found on the [releases page](https://github.com/RickP/MC5000_DevKit/releases).

The two MCU5000 ICs have to be flashed with different CPU-IDs:

MCU1: `easypdkprog -s 31 -n PFS173 write MC5000.PFS173.ihx`
MCU2: `easypdkprog -s 32 -n PFS173 write MC5000.PFS173.ihx`

The two peripheral controllers are just flashed without a cpu id:

BUZZER: `easypdkprog -n PFS173 write buzzer.PFS173.ihx`
DISPLAY: `easypdkprog -n PFS173 write display.PFS173.ihx`


## Desktop application

The desktop application transpiles the code to a binary format and uplloads it to the virtual 'MC5000' controllers on the board. It's written in QT5.12 and compiled automatically into packages for Linux, MacOS and Windows. Installation instructions are part of the [user documentation](https://rickp.github.io/MC5000_DevKit/).

The code is very simplistic with a lot of room for improvents. It's contained within the 'gui' subdirectory. The binaries can be found on the [releases page](https://github.com/RickP/MC5000_DevKit/releases).
