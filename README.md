# Overview

This is a statically linked Unikernel port of doomgeneric that runs natively on HermitOS.
The Doom WAD is embedded into the final binary, and runtime file access is overridden to read from in-memory data instead of a host filesystem.

# Prerequisites

- Docker (required): Use in conjunction with the [hermit-gcc crosscompiler image](https://github.com/hermit-os/hermit-gcc)
- Rust/Cargo (required): used to build the hermit-kernel (kernel-fork) via cargo xtask.
- QEMU (required): qemu-system-x86_64 is used to boot and run the image.
- curl (required): used to download the HermitOS multiboot loader. (You can however theoretically do this yourself and move it to the build folder)

# Game Assets

You can get a doom wad from the [Internet Archive](https://archive.org/download/doom-wads/Doom%20%28Demo%29.zip).
Place it at `wad/doom.wad`.

Only the shareware wad has been tested, you can however theoretically use any custom wad.
The build embeds this file directly into the binary (doom_wad.o) using x86_64-hermit-objcopy (see Makefile).

# Build & Run

Use `./build.sh` to build the Unikernel-Image and run Doom in QEMU.

# Controls

| Category | Key | Scancode (Hex) | Mapping / Action |
| --- | --- | --- | --- |
| **Movement** | W | `0x11` | `KEY_UPARROW` |
|  | A | `0x1E` | `KEY_LEFTARROW` |
|  | S | `0x1F` | `KEY_DOWNARROW` |
|  | D | `0x20` | `KEY_RIGHTARROW` |
| **Arrow keys** | Up | `0x48` | `KEY_UPARROW` |
|  | Down | `0x50` | `KEY_DOWNARROW` |
|  | Left | `0x4B` | `KEY_LEFTARROW` |
|  | Right | `0x4D` | `KEY_RIGHTARROW` |
| **Fire** | F | `0x21` | `KEY_FIRE` |
|  | Right Ctrl | `0x1D` | `KEY_RCTRL` |
|  | Shift | `0x2A` or `0x36` | `KEY_RSHIFT` |
| **Use** | Space | `0x39` | `KEY_USE` |
| **Menu navigation** | Enter | `0x1C` | `KEY_ENTER` |
|  | Escape | `0x01` | `KEY_ESCAPE` |
|  | Y (QWERTZ) | `0x2C` | `'y'` |
|  | N | `0x31` | `'n'` |

The Application prints out unknown scancodes into the console. Use the first (press) scancode to add custom keybinds in `doomgeneric_hermit.c`.
Scancodes can also be taken out of the [Scancode Set 1 on OSDev Wiki](https://wiki.osdev.org/PS/2_Keyboard#Scan_Code_Set_1).
