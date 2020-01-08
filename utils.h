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



// STRUCTURE POUR TABLE STRINGS
typedef struct _OneString OneString;

struct _OneString
{
	char t[25];
	int pos;
	OneString * suivant;
};

typedef struct
{
	OneString * tete;
	int nb;
} StringTab;



// STRUCTURE POUR TABLE REIMPLEMENTATIONS
typedef struct _OneReimp OneReimp;

struct _OneReimp
{
	int t[5];
	OneReimp * suivant;
};

typedef struct
{
	OneReimp * tete;
	int nb;
	int offset;
} ReimpTab;



// STRUCTURE POUR FILE
typedef struct
{
	Elf_Header h;
	SecHead s;
	SymTab st;
	StringTab string1;
	StringTab string2;
	ReimpTab r;
	FILE *l;
} OFile;




// PARTIE 1.1
Elf_Header read_header(FILE * f);
void print_header(Elf_Header h);


// PARTIE 1.2
SecHead read_section_headers(FILE * f, Elf_Header h);
void print_section_headers(SecHead s, StringTab string);


// PARTIE 1.3
void print_section(FILE * f, SecHead secHead, int i);
void write_section(FILE * lec, FILE *ec, SecHead secHead, int i);


// PARTIE 1.4
SymTab read_table_symboles(FILE * f, SecHead st);
void print_table_symboles(SymTab st);


// PARTIE 1.5
ReimpTab read_table_reimplantation(FILE * f, SecHead s, SymTab st);
void print_table_reimp(ReimpTab r);


// STRING TAB
StringTab read_string_table(FILE * f, Elf_Header head, SecHead s, int nb);
void print_string_tab(StringTab string);
char * string_pos(StringTab string, int pos);


// UTILS
void end(OFile a);

#endif

