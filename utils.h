#ifndef __UTILS_H__
#define __UTILS_H__

// STRUCTURE POUR HEADER
typedef struct
{
	Elf32_Ehdr h32;
	Elf64_Ehdr h64;
	int architecture;
	int endianess;
	int elf;
} Elf_Header;

// STRUCTURES POUR SECTIONS HEADER
typedef struct _OneHeader OneHeader;

struct _OneHeader
{
	Elf32_Shdr t;
	OneHeader * suivant;
};

typedef struct
{
	OneHeader * tete;
	int nb;
} SecHead;

// STRUCTURES POUR TABLE SYMBOLES
typedef struct _OneSymbol OneSymbol;

struct _OneSymbol
{
	Elf32_Sym t;
	OneSymbol * suivant;
};

typedef struct
{
	OneSymbol * tete;
	int nb;
} SymTab;



// PARTIE 1.1
Elf_Header read_header(FILE * f);
void print_header(Elf_Header h);

// PARTIE 1.2
SecHead read_section_headers(FILE * f, Elf_Header h);
void print_section_headers(SecHead s);

// PARTIE 1.3
void print_section(FILE * f, SecHead secHead, int i);

// PARTIE 1.4
SymTab read_table_symboles(FILE * f, SecHead st);
void print_table_symboles(SymTab st);

// PARTIE 1.5
void read_table_reimplantation(FILE * f, SecHead s, SymTab st);

// UTILS
void print_string_table(FILE * f, Elf_Header head, SecHead s);

void end(SecHead s, SymTab st);

#endif

