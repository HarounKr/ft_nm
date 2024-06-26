#pragma once

#include "../libft/libft.h"
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <elf.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct s_symbol_data {
    char *address;
    char type;
    char *name;
    bool   is_undefined;
} t_symbol_data;

typedef struct s_options{
    char **files_name;
    char file_name[256];
    int  files_nb;
    bool a; // Afficher tous les symboles, même ceux spécifiques au débogueur
    bool g; // N'afficher que les symboles externes (globaux + indefinis).
    bool u; // N'afficher que les symboles non définis (ceux externes à chaque fichier objet).
    bool r; // Renverser l'ordre de tri (numérique ou alphabétique) ; commencer par le dernier.
    bool p; // Ne pas trier les symboles, uniquement les afficher dans leur ordre de rencontre.
} t_options;

typedef struct s_elf_32 {
    char *strtab;
    char *shstrtab;
    int symbols_offset;
    int is_bigendian;
    Elf32_Shdr *sections_hdr; // Stock l'adresse des headers des sections
    Elf32_Sym *symtab; // Stock l'adresse du debut de la section strtab
    uint16_t e_shstrndx; // Index de la section de noms de section shstrtab
    uint16_t e_shnum;
    Elf32_Off e_shoff;
} t_elf_32;

typedef struct s_elf_64 {
    char *strtab;
    char *shstrtab;
    int symbols_offset;
    int is_bigendian;
    Elf64_Shdr *sections_hdr; // Stock l'adresse des headers des sections
    Elf64_Sym *symtab; // Stock l'adresse du debut de la section strtab
    uint16_t e_shstrndx; // Index de la section de noms de section shstrtab
    uint16_t e_shnum;
    Elf64_Off e_shoff;
} t_elf_64;

typedef struct s_archive_file_hdr {
    char file_identifier[16];  // Nom du fichier
    char timestamp[12];        // Timestamp
    char owner_id[6];          // Owner ID
    char group_id[6];          // Group ID
    char mode[8];              // Mode
    char file_size[10];        // Tail size
    char end_char[2]; 
} s_archive_file_hdr;

extern t_elf_32 elf_32;
extern t_elf_64 elf_64;
extern t_options options;

extern char *text_sections[6];
extern char *data_sections[10];
extern char *ro_sections[5];
extern char *weak_sections[4];

void    insertion_sort(t_symbol_data *array, int n, int reverse);
void    handle_output(t_symbol_data *sym_data, int sym_size);
void    help_output();
void    sym_data_init(t_symbol_data *sym_data, int size);

int     handle_elf_errors(Elf64_Ehdr *file_hdr, uint8_t *file_data, char *filename, long int st_size);
int     handle_elf_32(u_int8_t *file_data, t_elf_32 elf_32);
int     handle_elf_64(u_int8_t *file_data, t_elf_64 elf_64);
int     is_upper(char c);
int     print_error(char *file_name ,char *err, char *err_type, bool is_quote);
void    print_filename();

char    *formatted_address(uint64_t address, int index);
char    get_final_symbol_type(unsigned int type, unsigned int bind, unsigned int size, char *section_name);
char    *get_strtab(uint8_t *file_data, uint64_t strtab_size, Elf64_Off strtab_offset);

uint16_t convert_endian16(uint16_t value, unsigned char data_encoding);
uint32_t convert_endian32(uint32_t value, unsigned char data_encoding);
uint64_t convert_endian64(uint64_t value, unsigned char data_encoding);