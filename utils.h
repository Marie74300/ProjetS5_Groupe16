#ifndef __UTILS_H__
#define __UTILS_H__

typedef struct
{
	int t[10][10];
	int nb;
} SecHead;

Elf32_Ehdr read_header(FILE * f);
void print_header(Elf32_Ehdr h);

void read_section_headers(FILE * f, Elf32_Ehdr h, SecHead s);
void print_section_headers(SecHead s);

void print_section(FILE * f, int i);

void read_table_symboles(FILE * f, Elf32_Ehdr h);

#endif

