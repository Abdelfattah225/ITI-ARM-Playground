#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // For open() flags like O_RDONLY
#include <unistd.h>     // For read(), lseek(), close()
#include <elf.h>        // For ELF structures and constants
#include <stdint.h>     // For uint16_t, uint32_t, etc.
#include <stdlib.h>     // For malloc(), free()
#include <getopt.h>     // For getopt_long() - command line parsing
#include <string.h>

/* ============ Global Variables ============ */

/*
 * magic: Stores the first 5 bytes of the ELF file
 * - Bytes 0-3: Magic number (0x7f, 'E', 'L', 'F')
 * - Byte 4: Class (1 = 32-bit, 2 = 64-bit)
 */
unsigned char magic[5];

/*
 * ELF Headers for 32-bit and 64-bit files
 * These structures are defined in <elf.h>
 */
Elf64_Ehdr ELF_Header64;
Elf32_Ehdr ELF_Header32;

/* ============ Options Structure ============ */

/*
 * Options: Stores command line flags
 * Each field corresponds to a command line option
 * 1 = option is enabled, 0 = disabled
 */
typedef struct {
    int print_header;      // -h: Print ELF header
    int print_sections;    // -S: Print section headers
    int print_programs;    // -l: Print program headers
    int print_symbols;     // -s: Print symbol table (.symtab)
    int print_dynsymbols;  // -d: Print dynamic symbols (.dynsym)
    int print_all;         // -a: Print everything
} Options;

/* ============ Helper Functions ============ */

/*
 * get_elf_type: Converts ELF type number to human-readable string
 * 
 * @param type: The e_type field from ELF header
 * @return: String describing the file type
 * 
 * Common types:
 * - ET_REL: Relocatable file (.o files from compilation)
 * - ET_EXEC: Executable file (traditional executables)
 * - ET_DYN: Shared object (libraries and PIE executables)
 * - ET_CORE: Core dump (crash dump)
 */
const char *get_elf_type(uint16_t type)
{
    switch (type)
    {
    case ET_NONE:
        return "NONE (No file type)";
    case ET_REL:
        return "REL (Relocatable file)";
    case ET_EXEC:
        return "EXEC (Executable file)";
    case ET_DYN:
        return "DYN (Shared object file)";
    case ET_CORE:
        return "CORE (Core file)";
    default:
        return "Unknown";
    }
}

/*
 * get_machine_name: Converts machine architecture number to string
 * 
 * @param machine: The e_machine field from ELF header
 * @return: String describing the CPU architecture
 * 
 * This tells us what processor the binary was compiled for
 */
const char *get_machine_name(uint16_t machine)
{
    switch (machine)
    {
    case EM_NONE:
        return "No machine";
    case EM_386:
        return "Intel 80386 (x86)";
    case EM_ARM:
        return "ARM";
    case EM_X86_64:
        return "AMD x86-64";
    case EM_AARCH64:
        return "ARM AARCH64";
    case EM_RISCV:
        return "RISC-V";
    case EM_MIPS:
        return "MIPS";
    case EM_PPC:
        return "PowerPC";
    case EM_PPC64:
        return "PowerPC 64-bit";
    case EM_SPARC:
        return "SPARC";
    case EM_IA_64:
        return "Intel IA-64";
    default:
        return "Unknown";
    }
}

/*
 * get_osabi_name: Converts OS/ABI identifier to string
 * 
 * @param osabi: The e_ident[EI_OSABI] byte from ELF header
 * @return: String describing the target OS
 * 
 * Most Linux binaries use ELFOSABI_NONE (System V)
 */
const char *get_osabi_name(unsigned char osabi)
{
    switch (osabi)
    {
    case ELFOSABI_NONE:
        return "UNIX - System V";
    case ELFOSABI_HPUX:
        return "HP-UX";
    case ELFOSABI_NETBSD:
        return "NetBSD";
    case ELFOSABI_GNU:
        return "GNU/Linux";
    case ELFOSABI_SOLARIS:
        return "Solaris";
    case ELFOSABI_AIX:
        return "AIX";
    case ELFOSABI_FREEBSD:
        return "FreeBSD";
    case ELFOSABI_OPENBSD:
        return "OpenBSD";
    default:
        return "Unknown";
    }
}

/*
 * get_data_encoding: Converts endianness byte to string
 * 
 * @param encoding: The e_ident[EI_DATA] byte from ELF header
 * @return: String describing byte order
 * 
 * - Little endian: Least significant byte first (x86, ARM)
 * - Big endian: Most significant byte first (some MIPS, PowerPC)
 */
const char *get_data_encoding(unsigned char encoding)
{
    switch (encoding)
    {
    case ELFDATANONE:
        return "Invalid data encoding";
    case ELFDATA2LSB:
        return "2's complement, little endian";
    case ELFDATA2MSB:
        return "2's complement, big endian";
    default:
        return "Unknown";
    }
}

/*
 * get_section_type: Converts section type number to string
 * 
 * @param type: The sh_type field from section header
 * @return: String describing section type
 * 
 * Important section types:
 * - SHT_PROGBITS: Code or data (.text, .data, .rodata)
 * - SHT_SYMTAB: Symbol table (function/variable names)
 * - SHT_STRTAB: String table (stores actual strings)
 * - SHT_DYNSYM: Dynamic symbol table (for shared libraries)
 * - SHT_NOBITS: Section that occupies no file space (.bss)
 */
const char *get_section_type(uint32_t type)
{
    switch (type)
    {
    case SHT_NULL:
        return "NULL";
    case SHT_PROGBITS:
        return "PROGBITS";
    case SHT_SYMTAB:
        return "SYMTAB";
    case SHT_STRTAB:
        return "STRTAB";
    case SHT_RELA:
        return "RELA";
    case SHT_HASH:
        return "HASH";
    case SHT_DYNAMIC:
        return "DYNAMIC";
    case SHT_NOTE:
        return "NOTE";
    case SHT_NOBITS:
        return "NOBITS";
    case SHT_REL:
        return "REL";
    case SHT_SHLIB:
        return "SHLIB";
    case SHT_DYNSYM:
        return "DYNSYM";
    case SHT_INIT_ARRAY:
        return "INIT_ARRAY";
    case SHT_FINI_ARRAY:
        return "FINI_ARRAY";
    case SHT_PREINIT_ARRAY:
        return "PREINIT_ARRAY";
    case SHT_GROUP:
        return "GROUP";
    case SHT_SYMTAB_SHNDX:
        return "SYMTAB_SHNDX";
    case SHT_GNU_HASH:
        return "GNU_HASH";
    case SHT_GNU_versym:
        return "VERSYM";
    case SHT_GNU_verneed:
        return "VERNEED";
    case SHT_GNU_verdef:
        return "VERDEF";
    default:
        return "UNKNOWN";
    }
}

/*
 * get_segment_type: Converts program header type to string
 * 
 * @param type: The p_type field from program header
 * @return: String describing segment type
 * 
 * Important segment types:
 * - PT_LOAD: Loadable segment (actual code/data loaded into memory)
 * - PT_DYNAMIC: Dynamic linking information
 * - PT_INTERP: Path to program interpreter (e.g., /lib/ld-linux.so)
 * - PT_GNU_STACK: Stack permissions (executable stack or not)
 */
const char *get_segment_type(uint32_t type)
{
    switch (type)
    {
    case PT_NULL:
        return "NULL";
    case PT_LOAD:
        return "LOAD";
    case PT_DYNAMIC:
        return "DYNAMIC";
    case PT_INTERP:
        return "INTERP";
    case PT_NOTE:
        return "NOTE";
    case PT_SHLIB:
        return "SHLIB";
    case PT_PHDR:
        return "PHDR";
    case PT_TLS:
        return "TLS";
    case PT_GNU_EH_FRAME:
        return "GNU_EH_FRAME";
    case PT_GNU_STACK:
        return "GNU_STACK";
    case PT_GNU_RELRO:
        return "GNU_RELRO";
    case PT_GNU_PROPERTY:
        return "GNU_PROPERTY";
    default:
        return "UNKNOWN";
    }
}

/*
 * get_segment_flags: Converts segment flags to R/W/E string
 * 
 * @param flags: The p_flags field from program header
 * @return: String like "R E" (readable, executable) or "RW " (read/write)
 * 
 * Flags:
 * - PF_R (4): Readable
 * - PF_W (2): Writable
 * - PF_X (1): Executable
 * 
 * Note: Uses static buffer - not thread safe!
 */
const char *get_segment_flags(uint32_t flags)
{
    static char flag_str[4];  // Static so it persists after function returns
    flag_str[0] = (flags & PF_R) ? 'R' : ' ';  // Check read bit
    flag_str[1] = (flags & PF_W) ? 'W' : ' ';  // Check write bit
    flag_str[2] = (flags & PF_X) ? 'E' : ' ';  // Check execute bit (E for Execute)
    flag_str[3] = '\0';                         // Null terminator
    return flag_str;
}

/*
 * get_symbol_type: Extracts and converts symbol type from st_info
 * 
 * @param info: The st_info field from symbol entry
 * @return: String describing symbol type
 * 
 * Symbol types:
 * - STT_FUNC: Function (code)
 * - STT_OBJECT: Data object (variable)
 * - STT_FILE: Source file name
 * - STT_SECTION: Section symbol
 * 
 * Note: st_info contains both type (low 4 bits) and binding (high 4 bits)
 * ELF64_ST_TYPE() macro extracts the type
 */
const char *get_symbol_type(unsigned char info)
{
    switch (ELF64_ST_TYPE(info))  // Extract type from lower 4 bits
    {
    case STT_NOTYPE:
        return "NOTYPE";
    case STT_OBJECT:
        return "OBJECT";
    case STT_FUNC:
        return "FUNC";
    case STT_SECTION:
        return "SECTION";
    case STT_FILE:
        return "FILE";
    case STT_COMMON:
        return "COMMON";
    case STT_TLS:
        return "TLS";
    default:
        return "UNKNOWN";
    }
}

/*
 * get_symbol_binding: Extracts and converts symbol binding from st_info
 * 
 * @param info: The st_info field from symbol entry
 * @return: String describing symbol binding
 * 
 * Binding types:
 * - STB_LOCAL: Only visible within the file (static functions)
 * - STB_GLOBAL: Visible everywhere (exported functions)
 * - STB_WEAK: Like global, but can be overridden
 * 
 * ELF64_ST_BIND() macro extracts the binding from high 4 bits
 */
const char *get_symbol_binding(unsigned char info)
{
    switch (ELF64_ST_BIND(info))  // Extract binding from upper 4 bits
    {
    case STB_LOCAL:
        return "LOCAL";
    case STB_GLOBAL:
        return "GLOBAL";
    case STB_WEAK:
        return "WEAK";
    default:
        return "UNKNOWN";
    }
}

/* ============ Print Functions ============ */

/*
 * print_elf64_header: Prints all fields of 64-bit ELF header
 * 
 * @param hdr: Pointer to Elf64_Ehdr structure
 * 
 * The ELF header contains:
 * - e_ident[]: Magic number and file properties
 * - e_type: File type (executable, shared object, etc.)
 * - e_machine: Target architecture
 * - e_entry: Entry point address (where execution starts)
 * - e_phoff: Program header table offset
 * - e_shoff: Section header table offset
 * - e_phnum: Number of program headers
 * - e_shnum: Number of section headers
 * - e_shstrndx: Index of section containing section names
 */
void print_elf64_header(Elf64_Ehdr *hdr)
{
    printf("\n=============== ELF Header (64-bit) ===============\n\n");

    // Print magic bytes (first 16 bytes of e_ident)
    printf("Magic:                             ");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        printf("%02x ", hdr->e_ident[i]);
    }
    printf("\n");

    // Print decoded e_ident fields
    printf("Class:                             ELF64\n");
    printf("Data:                              %s\n", get_data_encoding(hdr->e_ident[EI_DATA]));
    printf("Version:                           %d (current)\n", hdr->e_ident[EI_VERSION]);
    printf("OS/ABI:                            %s\n", get_osabi_name(hdr->e_ident[EI_OSABI]));
    printf("ABI Version:                       %d\n", hdr->e_ident[EI_ABIVERSION]);
    
    // Print main header fields
    printf("Type:                              %s\n", get_elf_type(hdr->e_type));
    printf("Machine:                           %s\n", get_machine_name(hdr->e_machine));
    printf("Version:                           0x%x\n", hdr->e_version);
    printf("Entry point address:               0x%lx\n", hdr->e_entry);
    printf("Start of program headers:          %lu (bytes into file)\n", hdr->e_phoff);
    printf("Start of section headers:          %lu (bytes into file)\n", hdr->e_shoff);
    printf("Flags:                             0x%x\n", hdr->e_flags);
    printf("Size of this header:               %d (bytes)\n", hdr->e_ehsize);
    printf("Size of program headers:           %d (bytes)\n", hdr->e_phentsize);
    printf("Number of program headers:         %d\n", hdr->e_phnum);
    printf("Size of section headers:           %d (bytes)\n", hdr->e_shentsize);
    printf("Number of section headers:         %d\n", hdr->e_shnum);
    printf("Section header string table index: %d\n", hdr->e_shstrndx);

    printf("\n===================================================\n");
}

/*
 * print_elf32_header: Prints all fields of 32-bit ELF header
 * 
 * @param hdr: Pointer to Elf32_Ehdr structure
 * 
 * Same as 64-bit version but uses 32-bit data types
 * Note: %u and %x for 32-bit values vs %lu and %lx for 64-bit
 */
void print_elf32_header(Elf32_Ehdr *hdr)
{
    printf("\n=============== ELF Header (32-bit) ===============\n\n");

    printf("Magic:                             ");
    for (int i = 0; i < EI_NIDENT; i++)
    {
        printf("%02x ", hdr->e_ident[i]);
    }
    printf("\n");

    printf("Class:                             ELF32\n");
    printf("Data:                              %s\n", get_data_encoding(hdr->e_ident[EI_DATA]));
    printf("Version:                           %d (current)\n", hdr->e_ident[EI_VERSION]);
    printf("OS/ABI:                            %s\n", get_osabi_name(hdr->e_ident[EI_OSABI]));
    printf("ABI Version:                       %d\n", hdr->e_ident[EI_ABIVERSION]);
    printf("Type:                              %s\n", get_elf_type(hdr->e_type));
    printf("Machine:                           %s\n", get_machine_name(hdr->e_machine));
    printf("Version:                           0x%x\n", hdr->e_version);
    printf("Entry point address:               0x%x\n", hdr->e_entry);
    printf("Start of program headers:          %u (bytes into file)\n", hdr->e_phoff);
    printf("Start of section headers:          %u (bytes into file)\n", hdr->e_shoff);
    printf("Flags:                             0x%x\n", hdr->e_flags);
    printf("Size of this header:               %d (bytes)\n", hdr->e_ehsize);
    printf("Size of program headers:           %d (bytes)\n", hdr->e_phentsize);
    printf("Number of program headers:         %d\n", hdr->e_phnum);
    printf("Size of section headers:           %d (bytes)\n", hdr->e_shentsize);
    printf("Number of section headers:         %d\n", hdr->e_shnum);
    printf("Section header string table index: %d\n", hdr->e_shstrndx);

    printf("\n===================================================\n");
}

/*
 * print_section_headers_64: Prints all section headers for 64-bit ELF
 * 
 * @param sections: Array of Elf64_Shdr structures
 * @param count: Number of sections
 * @param strtab: Pointer to section string table (for section names)
 * 
 * Each section header contains:
 * - sh_name: Offset into string table for section name
 * - sh_type: Section type (PROGBITS, SYMTAB, etc.)
 * - sh_addr: Virtual address where section is loaded
 * - sh_offset: Offset of section data in file
 * - sh_size: Size of section in bytes
 */
void print_section_headers_64(Elf64_Shdr *sections, int count, char *strtab)
{
    printf("\n=============== Section Headers (64-bit) ===============\n\n");
    printf("[Nr] %-18s %-15s %-16s %-8s %-8s\n",
           "Name", "Type", "Address", "Offset", "Size");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        // Get section name from string table
        // sh_name is an OFFSET into strtab, not the actual name!
        char *name = (strtab != NULL) ? &strtab[sections[i].sh_name] : "(no name)";

        printf("[%2d] %-18s %-15s %016lx %08lx %08lx\n",
               i,
               name,
               get_section_type(sections[i].sh_type),
               sections[i].sh_addr,
               sections[i].sh_offset,
               sections[i].sh_size);
    }

    printf("\n========================================================\n");
}

/*
 * print_section_headers_32: Same as 64-bit version but for 32-bit ELF
 */
void print_section_headers_32(Elf32_Shdr *sections, int count, char *strtab)
{
    printf("\n=============== Section Headers (32-bit) ===============\n\n");
    printf("[Nr] %-18s %-15s %-10s %-8s %-8s\n",
           "Name", "Type", "Address", "Offset", "Size");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        char *name = (strtab != NULL) ? &strtab[sections[i].sh_name] : "(no name)";

        printf("[%2d] %-18s %-15s %08x %08x %08x\n",
               i,
               name,
               get_section_type(sections[i].sh_type),
               sections[i].sh_addr,
               sections[i].sh_offset,
               sections[i].sh_size);
    }

    printf("\n========================================================\n");
}

/*
 * print_program_headers_64: Prints all program headers for 64-bit ELF
 * 
 * @param phdrs: Array of Elf64_Phdr structures
 * @param count: Number of program headers
 * 
 * Program headers describe SEGMENTS - how the OS loads the file into memory
 * Each program header contains:
 * - p_type: Segment type (LOAD, DYNAMIC, INTERP, etc.)
 * - p_offset: Offset of segment in file
 * - p_vaddr: Virtual address where segment is loaded
 * - p_filesz: Size of segment in file
 * - p_memsz: Size of segment in memory (can be > filesz for .bss)
 * - p_flags: Permissions (read, write, execute)
 * - p_align: Alignment requirement
 */
void print_program_headers_64(Elf64_Phdr *phdrs, int count)
{
    printf("\n=============== Program Headers (64-bit) ===============\n\n");
    printf("%-14s %-18s %-18s %-10s\n",
           "Type", "Offset", "VirtAddr", "Flags");
    printf("%-14s %-18s %-18s %-10s\n",
           "", "FileSiz", "MemSiz", "Align");
    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        // First line: type, offset, virtual address, flags
        printf("%-14s 0x%016lx 0x%016lx %s\n",
               get_segment_type(phdrs[i].p_type),
               phdrs[i].p_offset,
               phdrs[i].p_vaddr,
               get_segment_flags(phdrs[i].p_flags));

        // Second line: file size, memory size, alignment
        printf("%-14s 0x%016lx 0x%016lx 0x%lx\n",
               "",
               phdrs[i].p_filesz,
               phdrs[i].p_memsz,
               phdrs[i].p_align);
        printf("\n");
    }

    printf("========================================================\n");
}

/*
 * print_program_headers_32: Same as 64-bit version but for 32-bit ELF
 */
void print_program_headers_32(Elf32_Phdr *phdrs, int count)
{
    printf("\n=============== Program Headers (32-bit) ===============\n\n");
    printf("%-14s %-10s %-10s %-10s %-6s\n",
           "Type", "Offset", "VirtAddr", "PhysAddr", "Flags");
    printf("%-14s %-10s %-10s %-10s\n",
           "", "FileSiz", "MemSiz", "Align");
    printf("----------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        printf("%-14s 0x%08x 0x%08x 0x%08x %s\n",
               get_segment_type(phdrs[i].p_type),
               phdrs[i].p_offset,
               phdrs[i].p_vaddr,
               phdrs[i].p_paddr,
               get_segment_flags(phdrs[i].p_flags));

        printf("%-14s 0x%08x 0x%08x 0x%x\n",
               "",
               phdrs[i].p_filesz,
               phdrs[i].p_memsz,
               phdrs[i].p_align);
        printf("\n");
    }

    printf("========================================================\n");
}

/*
 * print_symbols_64: Prints symbol table entries for 64-bit ELF
 * 
 * @param symbols: Array of Elf64_Sym structures
 * @param count: Number of symbols
 * @param strtab: Pointer to symbol string table (for symbol names)
 * @param table_name: Name to display (e.g., "Symbol Table (.symtab)")
 * 
 * Each symbol entry contains:
 * - st_name: Offset into string table for symbol name
 * - st_value: Symbol value (address for functions/variables)
 * - st_size: Size of symbol (e.g., function size in bytes)
 * - st_info: Type and binding (packed into one byte)
 */
void print_symbols_64(Elf64_Sym *symbols, int count, char *strtab, const char *table_name)
{
    printf("\n=============== %s (64-bit) ===============\n\n", table_name);
    printf("[Nr] %-30s %-16s %-8s %-10s %-8s\n",
           "Name", "Value", "Size", "Type", "Bind");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        // Get symbol name from string table
        // Empty string if st_name is 0 (null symbol)
        char *name = "";
        if (strtab != NULL && symbols[i].st_name != 0)
        {
            name = &strtab[symbols[i].st_name];
        }

        printf("[%3d] %-30s %016lx %08lx %-10s %-8s\n",
               i,
               name,
               symbols[i].st_value,
               symbols[i].st_size,
               get_symbol_type(symbols[i].st_info),
               get_symbol_binding(symbols[i].st_info));
    }

    printf("\n========================================================\n");
}

/*
 * print_symbols_32: Same as 64-bit version but for 32-bit ELF
 */
void print_symbols_32(Elf32_Sym *symbols, int count, char *strtab, const char *table_name)
{
    printf("\n=============== %s (32-bit) ===============\n\n", table_name);
    printf("[Nr] %-30s %-10s %-8s %-10s %-8s\n",
           "Name", "Value", "Size", "Type", "Bind");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {
        char *name = "";
        if (strtab != NULL && symbols[i].st_name != 0)
        {
            name = &strtab[symbols[i].st_name];
        }

        printf("[%3d] %-30s %08x %08x %-10s %-8s\n",
               i,
               name,
               symbols[i].st_value,
               symbols[i].st_size,
               get_symbol_type(symbols[i].st_info),
               get_symbol_binding(symbols[i].st_info));
    }

    printf("\n========================================================\n");
}

/*
 * print_usage: Displays help message with all available options
 * 
 * @param prog_name: Name of the program (argv[0])
 */
void print_usage(const char *prog_name)
{
    printf("Usage: %s [OPTIONS] <elf_file>\n\n", prog_name);
    printf("Options:\n");
    printf("  -h          Display ELF header\n");
    printf("  -S          Display section headers\n");
    printf("  -l          Display program headers\n");
    printf("  -s          Display symbol table\n");
    printf("  -d          Display dynamic symbol table\n");
    printf("  -a          Display all (equivalent to -h -S -l -s -d)\n");
    printf("  --help      Display this help message\n");
    printf("\nExamples:\n");
    printf("  %s -h /bin/ls           Show ELF header only\n", prog_name);
    printf("  %s -S -s /bin/ls        Show sections and symbols\n", prog_name);
    printf("  %s -a /bin/ls           Show everything\n", prog_name);
}

/* ============ Read Symbol Table Functions ============ */

/*
 * read_and_print_symbols_64: Reads and prints a symbol table section
 * 
 * @param fd: File descriptor of the ELF file
 * @param sections: Array of all section headers
 * @param section_idx: Index of the symbol table section
 * @param table_name: Name to display in output
 * 
 * This function:
 * 1. Calculates number of symbols (section size / symbol size)
 * 2. Allocates memory and reads symbol table
 * 3. Uses sh_link to find the associated string table
 * 4. Reads string table for symbol names
 * 5. Prints all symbols
 */
void read_and_print_symbols_64(int fd, Elf64_Shdr *sections, int section_idx, const char *table_name)
{
    // Calculate number of symbols
    // Each symbol is sizeof(Elf64_Sym) bytes
    int num_symbols = sections[section_idx].sh_size / sizeof(Elf64_Sym);

    // Allocate memory for all symbols
    Elf64_Sym *symbols = (Elf64_Sym *)malloc(sections[section_idx].sh_size);
    if (symbols == NULL)
    {
        perror("malloc failed for symbols");
        return;
    }

    // Seek to symbol table offset and read all symbols
    lseek(fd, sections[section_idx].sh_offset, SEEK_SET);
    ssize_t bytes_read = read(fd, symbols, sections[section_idx].sh_size);
    if (bytes_read != (ssize_t)sections[section_idx].sh_size)
    {
        perror("Failed to read symbol table");
        free(symbols);
        return;
    }

    // Read the associated string table
    // sh_link field tells us which section contains symbol names
    char *sym_strtab = NULL;
    if (sections[section_idx].sh_link != SHN_UNDEF)
    {
        // Get the string table section header
        Elf64_Shdr *sym_strtab_hdr = &sections[sections[section_idx].sh_link];
        
        // Allocate and read the string table
        sym_strtab = (char *)malloc(sym_strtab_hdr->sh_size);
        if (sym_strtab != NULL)
        {
            lseek(fd, sym_strtab_hdr->sh_offset, SEEK_SET);
            read(fd, sym_strtab, sym_strtab_hdr->sh_size);
        }
    }

    // Print all symbols
    print_symbols_64(symbols, num_symbols, sym_strtab, table_name);

    // Clean up
    free(sym_strtab);
    free(symbols);
}

/*
 * read_and_print_symbols_32: Same as 64-bit version but for 32-bit ELF
 */
void read_and_print_symbols_32(int fd, Elf32_Shdr *sections, int section_idx, const char *table_name)
{
    int num_symbols = sections[section_idx].sh_size / sizeof(Elf32_Sym);

    Elf32_Sym *symbols = (Elf32_Sym *)malloc(sections[section_idx].sh_size);
    if (symbols == NULL)
    {
        perror("malloc failed for symbols");
        return;
    }

    lseek(fd, sections[section_idx].sh_offset, SEEK_SET);
    ssize_t bytes_read = read(fd, symbols, sections[section_idx].sh_size);
    if (bytes_read != (ssize_t)sections[section_idx].sh_size)
    {
        perror("Failed to read symbol table");
        free(symbols);
        return;
    }

    char *sym_strtab = NULL;
    if (sections[section_idx].sh_link != SHN_UNDEF)
    {
        Elf32_Shdr *sym_strtab_hdr = &sections[sections[section_idx].sh_link];
        sym_strtab = (char *)malloc(sym_strtab_hdr->sh_size);
        if (sym_strtab != NULL)
        {
            lseek(fd, sym_strtab_hdr->sh_offset, SEEK_SET);
            read(fd, sym_strtab, sym_strtab_hdr->sh_size);
        }
    }

    print_symbols_32(symbols, num_symbols, sym_strtab, table_name);

    free(sym_strtab);
    free(symbols);
}

/* ============ Main Function ============ */

int main(int argc, char *argv[])
{
    Options opts = {0};  // Initialize all options to 0 (disabled)
    int opt;
    char *filename = NULL;

    /*
     * Define long options for getopt_long()
     * Format: {name, has_arg, flag, val}
     * - name: Long option name (--help)
     * - has_arg: no_argument, required_argument, or optional_argument
     * - flag: If NULL, getopt_long returns val
     * - val: Value to return or store
     */
    static struct option long_options[] = {
        {"help", no_argument, 0, 'H'},  // --help returns 'H'
        {0, 0, 0, 0}                     // End of options marker
    };

    /*
     * Parse command line options using getopt_long()
     * 
     * "hSlsda" means:
     * - h: -h option (no argument)
     * - S: -S option (no argument)
     * - l: -l option (no argument)
     * - s: -s option (no argument)
     * - d: -d option (no argument)
     * - a: -a option (no argument)
     * 
     * getopt_long() returns -1 when all options are processed
     */
    while ((opt = getopt_long(argc, argv, "hSlsda", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            opts.print_header = 1;
            break;
        case 'S':
            opts.print_sections = 1;
            break;
        case 'l':
            opts.print_programs = 1;
            break;
        case 's':
            opts.print_symbols = 1;
            break;
        case 'd':
            opts.print_dynsymbols = 1;
            break;
        case 'a':
            opts.print_all = 1;
            break;
        case 'H':  // --help
            print_usage(argv[0]);
            return 0;
        default:
            print_usage(argv[0]);
            return 1;
        }
    }

    /*
     * Get filename from remaining arguments
     * optind is the index of the first non-option argument
     * After parsing "-h -S", optind points to the filename
     */
    if (optind < argc)
    {
        filename = argv[optind];
    }
    else
    {
        printf("Error: No ELF file specified.\n\n");
        print_usage(argv[0]);
        return 1;
    }

    // If -a (all) is specified, enable all options
    if (opts.print_all)
    {
        opts.print_header = 1;
        opts.print_sections = 1;
        opts.print_programs = 1;
        opts.print_symbols = 1;
        opts.print_dynsymbols = 1;
    }

    // If no display option specified, show error
    if (!opts.print_header && !opts.print_sections && !opts.print_programs &&
        !opts.print_symbols && !opts.print_dynsymbols)
    {
        printf("Error: No display option specified.\n\n");
        print_usage(argv[0]);
        return 1;
    }

    /*
     * Open the ELF file
     * O_RDONLY: Open for reading only
     * Returns file descriptor (fd) on success, -1 on error
     */
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Failed to open the file");
        return 1;
    }

    /*
     * Read and validate ELF magic number
     * First 4 bytes must be: 0x7f 'E' 'L' 'F'
     * 5th byte indicates 32-bit (1) or 64-bit (2)
     */
    ssize_t bytes_read = read(fd, magic, 5);
    if (bytes_read != 5)
    {
        perror("Failed to read file");
        close(fd);
        return 1;
    }

    // Validate magic number
    if (magic[0] != 0x7f || magic[1] != 'E' || magic[2] != 'L' || magic[3] != 'F')
    {
        printf("Invalid ELF file: wrong magic number\n");
        close(fd);
        return 1;
    }

    // Reset file position to beginning for reading full header
    lseek(fd, 0, SEEK_SET);

    /* ========== Handle 32-bit ELF ========== */
    if (magic[4] == 1)
    {
        // Read 32-bit ELF header
        bytes_read = read(fd, &ELF_Header32, sizeof(Elf32_Ehdr));
        if (bytes_read != sizeof(Elf32_Ehdr))
        {
            perror("Failed to read ELF header 32-bit");
            close(fd);
            return 1;
        }

        // Print header if requested
        if (opts.print_header)
        {
            print_elf32_header(&ELF_Header32);
        }

        /*
         * Read section headers if needed for any option
         * Section headers are needed for:
         * - Printing sections (-S)
         * - Finding symbol tables (-s, -d)
         */
        Elf32_Shdr *sections = NULL;
        char *strtab = NULL;

        if (opts.print_sections || opts.print_symbols || opts.print_dynsymbols)
        {
            // Allocate memory for all section headers
            sections = (Elf32_Shdr *)malloc(ELF_Header32.e_shnum * sizeof(Elf32_Shdr));
            if (sections == NULL)
            {
                perror("malloc failed");
                close(fd);
                return 1;
            }

            // Seek to section header table and read all headers
            lseek(fd, ELF_Header32.e_shoff, SEEK_SET);
            ssize_t total_size = ELF_Header32.e_shentsize * ELF_Header32.e_shnum;
            bytes_read = read(fd, sections, total_size);
            if (bytes_read != total_size)
            {
                perror("Failed to read section headers");
                free(sections);
                close(fd);
                return 1;
            }

            /*
             * Read section name string table
             * e_shstrndx tells us which section contains section names
             * This is different from symbol string tables!
             */
            if (ELF_Header32.e_shstrndx != SHN_UNDEF)
            {
                Elf32_Shdr *strtab_hdr = &sections[ELF_Header32.e_shstrndx];
                strtab = (char *)malloc(strtab_hdr->sh_size);
                if (strtab != NULL)
                {
                    lseek(fd, strtab_hdr->sh_offset, SEEK_SET);
                    read(fd, strtab, strtab_hdr->sh_size);
                }
            }
        }

        // Print sections if requested
        if (opts.print_sections && sections)
        {
            print_section_headers_32(sections, ELF_Header32.e_shnum, strtab);
        }

        // Print program headers if requested
        if (opts.print_programs)
        {
            Elf32_Phdr *phdrs = (Elf32_Phdr *)malloc(ELF_Header32.e_phnum * sizeof(Elf32_Phdr));
            if (phdrs != NULL)
            {
                lseek(fd, ELF_Header32.e_phoff, SEEK_SET);
                ssize_t ph_total_size = ELF_Header32.e_phentsize * ELF_Header32.e_phnum;
                bytes_read = read(fd, phdrs, ph_total_size);
                if (bytes_read == ph_total_size)
                {
                    print_program_headers_32(phdrs, ELF_Header32.e_phnum);
                }
                free(phdrs);
            }
        }

        /*
         * Print symbols if requested
         * Loop through all sections to find symbol tables
         * - SHT_SYMTAB: Full symbol table (.symtab) - may be stripped
         * - SHT_DYNSYM: Dynamic symbols (.dynsym) - always present
         */
        if ((opts.print_symbols || opts.print_dynsymbols) && sections)
        {
            for (int i = 0; i < ELF_Header32.e_shnum; i++)
            {
                // Print .symtab if -s specified
                if (opts.print_symbols && sections[i].sh_type == SHT_SYMTAB)
                {
                    read_and_print_symbols_32(fd, sections, i, "Symbol Table (.symtab)");
                }
                // Print .dynsym if -d specified
                if (opts.print_dynsymbols && sections[i].sh_type == SHT_DYNSYM)
                {
                    read_and_print_symbols_32(fd, sections, i, "Dynamic Symbol Table (.dynsym)");
                }
            }
        }

        // Clean up allocated memory
        free(strtab);
        free(sections);
    }
    /* ========== Handle 64-bit ELF ========== */
    else if (magic[4] == 2)
    {
        // Read 64-bit ELF header
        bytes_read = read(fd, &ELF_Header64, sizeof(Elf64_Ehdr));
        if (bytes_read != sizeof(Elf64_Ehdr))
        {
            perror("Failed to read ELF header 64-bit");
            close(fd);
            return 1;
        }

        // Print header if requested
        if (opts.print_header)
        {
            print_elf64_header(&ELF_Header64);
        }

        // Read section headers if needed
        Elf64_Shdr *sections = NULL;
        char *strtab = NULL;

        if (opts.print_sections || opts.print_symbols || opts.print_dynsymbols)
        {
            sections = (Elf64_Shdr *)malloc(ELF_Header64.e_shnum * sizeof(Elf64_Shdr));
            if (sections == NULL)
            {
                perror("malloc failed");
                close(fd);
                return 1;
            }

            lseek(fd, ELF_Header64.e_shoff, SEEK_SET);
            ssize_t total_size = ELF_Header64.e_shentsize * ELF_Header64.e_shnum;
            bytes_read = read(fd, sections, total_size);
            if (bytes_read != total_size)
            {
                perror("Failed to read section headers");
                free(sections);
                close(fd);
                return 1;
            }

            // Read section name string table
            if (ELF_Header64.e_shstrndx != SHN_UNDEF)
            {
                Elf64_Shdr *strtab_hdr = &sections[ELF_Header64.e_shstrndx];
                strtab = (char *)malloc(strtab_hdr->sh_size);
                if (strtab != NULL)
                {
                    lseek(fd, strtab_hdr->sh_offset, SEEK_SET);
                    read(fd, strtab, strtab_hdr->sh_size);
                }
            }
        }

        // Print sections if requested
        if (opts.print_sections && sections)
        {
            print_section_headers_64(sections, ELF_Header64.e_shnum, strtab);
        }

        // Print program headers if requested
        if (opts.print_programs)
        {
            Elf64_Phdr *phdrs = (Elf64_Phdr *)malloc(ELF_Header64.e_phnum * sizeof(Elf64_Phdr));
            if (phdrs != NULL)
            {
                lseek(fd, ELF_Header64.e_phoff, SEEK_SET);
                ssize_t ph_total_size = ELF_Header64.e_phentsize * ELF_Header64.e_phnum;
                bytes_read = read(fd, phdrs, ph_total_size);
                if (bytes_read == ph_total_size)
                {
                    print_program_headers_64(phdrs, ELF_Header64.e_phnum);
                }
                free(phdrs);
            }
        }

        // Print symbols if requested
        if ((opts.print_symbols || opts.print_dynsymbols) && sections)
        {
            for (int i = 0; i < ELF_Header64.e_shnum; i++)
            {
                if (opts.print_symbols && sections[i].sh_type == SHT_SYMTAB)
                {
                    read_and_print_symbols_64(fd, sections, i, "Symbol Table (.symtab)");
                }
                if (opts.print_dynsymbols && sections[i].sh_type == SHT_DYNSYM)
                {
                    read_and_print_symbols_64(fd, sections, i, "Dynamic Symbol Table (.dynsym)");
                }
            }
        }

        // Clean up
        free(strtab);
        free(sections);
    }
    else
    {
        // Unknown ELF class (not 32-bit or 64-bit)
        printf("Unknown ELF class: %d\n", magic[4]);
        close(fd);
        return 1;
    }

    // Close file and exit
    close(fd);
    return 0;
}