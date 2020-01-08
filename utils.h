#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct
{
	Elf32_Shdr t[10];
	int nb;
} SecHead;

typedef struct
{
	Elf32_Sym t[100]; //Todo replace 100 with allocation
	int nb;
} SymTab;

// typedef struct
// {
// 	int name;
// 	int value;
// 	int size;
// 	int info;
// 	int other;
// 	int shndx;
// } SymTab;


Elf32_Ehdr read_header(FILE * f);
void print_header(Elf32_Ehdr h);

SecHead read_section_headers(FILE * f, Elf32_Ehdr h, SecHead s);
void print_section_headers(SecHead s);

void print_section(FILE * f, SecHead secHead, int i);

SymTab read_table_symboles(FILE * f, SecHead secHead, SymTab symTab);
void print_table_symboles(SymTab symTab);

void read_table_reimplantation(FILE * f, SecHead secHead, SymTab symTab);

void print_string_table(Elf32_Ehdr head, SecHead secHead);
#endif
