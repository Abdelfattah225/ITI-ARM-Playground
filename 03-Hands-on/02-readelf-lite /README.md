# readelf-lite ( ELF Parser )

A lightweight command-line tool to parse and display ELF (Executable and Linkable Format) file information, similar to `readelf`.

## Features

- ✅ ELF Header parsing
- ✅ Section Headers
- ✅ Program Headers
- ✅ Symbol Table (.symtab)
- ✅ Dynamic Symbol Table (.dynsym)
- ✅ 32-bit and 64-bit support

## Build

```bash
gcc -o elf-parser elf_parser.c
```

## Usage

```bash
./elf-parser [OPTIONS] <elf_file>
```

### Options

| Option | Description |
|--------|-------------|
| `-h` | Display ELF header |
| `-S` | Display section headers |
| `-l` | Display program headers |
| `-s` | Display symbol table |
| `-d` | Display dynamic symbols |
| `-a` | Display all |
| `--help` | Show help message |

### Examples

```bash
# Show ELF header
./elf-parser -h /bin/ls

# Show sections and symbols
./elf-parser -S -s /bin/ls

# Show everything
./elf-parser -a /bin/ls

# Show dynamic symbols (works on stripped binaries)
./elf-parser -d /bin/ls
```

## Sample Output

```
=============== ELF Header (64-bit) ===============

Magic:                             7f 45 4c 46 02 01 01 00 ...
Class:                             ELF64
Type:                              DYN (Shared object file)
Machine:                           AMD x86-64
Entry point address:               0x6aa0
Number of section headers:         31
Number of program headers:         13

===================================================
```

## ELF Structure Overview

```
┌──────────────────┐
│    ELF Header    │  Magic, type, machine, entry point
├──────────────────┤
│ Program Headers  │  Segments (how to load into memory)
├──────────────────┤
│    Sections      │  .text, .data, .bss, .symtab
├──────────────────┤
│ Section Headers  │  Section metadata
└──────────────────┘
```

## Requirements

- Linux/Unix system
- GCC compiler
- `<elf.h>` header (standard on Linux)
