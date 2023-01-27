# Google Pixel Titan-M2 simulation

This repository contains some modifications to the spike riscv simulator:

 * added several device plugins, needed to boot the Titan-M2 firmware
 * added several custom instructions needed by the Titan-M2.
 * added several custom csr registers needed by the Titan-M2.
 * added a `--patch` commandline option
 * added a `set` command to the interactive shell.
 * disabled the 'debugmod'


## devices

The devices are far from functional, they only implement the minimum behaviour
to make the firmware believe them to be working.

The following devices have been added:

 * 0x40100000 - probably mmu type
    * needs specific values initialized, and other than that basically functions as RAM.
 * 0x40110000 - probably flash / dma
    * reacts to specific values being written, loading values in RAM elsewhere.
 * 0x404d0000 - probably a UART.
    * setting bits in specific register, so it is considered 'ready'
 * 0x40620000 - probably timer related
    * reacting to specific bits being set/cleared
 * 0x40630000 - timer
    * usec counter, automatically increments after each load. not real-time!!
 * 0x00000000 - bootrom
    * at 0x1000 some code for the reset entry point
    * at 0x00xx several jumps to unknown bootrom functions.
    * fn at 0x0070 is probably: calc-sha256

## custom instructions

There are several custom instructions used by the TitanM2.

Two of which I am sure what they do: byte-swap, and count-leading-zeros.

The others are probably similar bit fiddling instructions.

 * rbitscan
    * count-leading-zeroes
 * bitscan
    * similar, but probably trailing zeroes, or trailing ones
 * bswap32
    * reverse byte order of value in register.
 * google0
 * google1

This is how these instructions are encoded:

```
1098765 43210 98765 432 10987 6543210 
 func7   ?     rs1  func3  rd  opcode
0000000 11000 .rs1. 000 ..rd. 0001011  gbswap     rd, rs1
0000000 00000 .rs1. 010 ..rd. 0001011  gbitscan   rd, rs1
0100000 00000 .rs1. 010 ..rd. 0001011  grbitscan  rd, rs1
```

```
1098765 43210 98765 432 10987 6543210 
 func7  shamt  rs1 func3   rd  opcode
0000000 shamt .rs1. 001 ..rd. 0001011  google0a rd, rs1, #shamt
0100000 shamt .rs1. 001 ..rd. 0001011  google0b rd, rs1, #shamt
```

```
1098765 43210 98765 432 10987 6543210 
 func7   rs2   rs1 func3   rd  opcode
0000000 .rs2. .rs1. 001 ..rd. 0101011  google1a  rd, rs1, rs2
0100000 .rs2. .rs1. 001 ..rd. 0101011  google1b  rd, rs1, rs2 
```



## custom csr registers

Used by the firmware to keep pointers to several vector tables. Not used by the cpu, only by the firmware.

| csreg | description
| :--  | :--
| 0x7c0 | ecall function table
| 0x7c1 | 2nd ecall function table
| 0x7c2 | trap return address
| 0x7c4 | some bit flag register
| 0x7c5 | the current thread pointer
| 0x7d0 | some exception handler.
| 0x807 | unknown


## the `--patch` commandline option.

This allows you to specify values for memory locations on the commandline.


## the `set` interactive command.

This allows you to interrupt a running program, and modify memory locations.


## disabled `debugmod`.

This was in the way of the bootrom, I decided to completely disable it.
Also I commented out the `sim.cc` boot code, and replaced it with my own.


# running

First use the `mkelf.sh` script to convert a `evt.ec.bin` file to .elf suitable for spike.
Then use either `run.sh`, for running the 'rw' firmware, or 'run8.sh` for running the `ro` part of the firmware.

