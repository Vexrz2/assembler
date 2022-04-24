# assembler
Two-pass assembler written in C

This assembler consists of 3 files: source-files, example, and makefile.
The source-files folder consists of the source files (self-explanatory) for the assembler.
The example folder contains example inputs and outputs for the assembler.

This assembler works on a "virtual" computer in the sense that it doesn't actually work on a real computer. 
The "computer" uses 8192 blocks of memory (each block is 20 bits long) and has 16 registers (also 20 bits long)
The made-up language consists of 16 different instructions, and each instruction has a different amount of operands and addressing modes.

All in all, this program is an efficient 2-pass assembler on a made-up language which returns files for the external variables, and the binary code (represented in a special form) after assembly.
