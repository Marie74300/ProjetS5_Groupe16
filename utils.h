#ifndef __UTILS_H__
#define __UTILS_H__


// POWER
int power(int a, int b);
//calcul la puissance


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
	Elf32_Shdr t32;
	Elf64_Shdr t64;
	OneHeader * suivant;
};
typedef struct
{
	OneHeader * tete;
	int nb;
} SecHead;


// STRUCTURE POUR TABLE STRINGS
typedef struct _OneString OneString;
struct _OneString
{
	char c;
	OneString * suivant;
};
typedef struct
{
	OneString * tete;
	int nb;
} StringTab;


// STRUCTURES POUR TABLE SYMBOLES
typedef struct _OneSymbol OneSymbol;
struct _OneSymbol
{
	Elf32_Sym t32;
	Elf64_Sym t64;
	OneSymbol * suivant;
};
typedef struct
{
	OneSymbol * tete;
	int nb;
} SymTab;


// STRUCTURE POUR TABLE REIMPLEMENTATIONS
typedef struct _OneReimp OneReimp;
struct _OneReimp
{
	Elf32_Rel t32;
	Elf64_Rel t64;
	int value;
	int name;
	OneReimp * suivant;
};
typedef struct
{
	OneReimp * tete;
	int nb;
	int offset;
} ReimpTab;
// STRUCTURE POUR LISTE TABLE REIMPLEMETATIONS
typedef struct _OneList OneList;
struct _OneList
{
	ReimpTab r;
	OneList * suivant;
};
typedef struct
{
	OneList  * tete;
	int nb;
} ListReimpTab;


// STRUCTURE POUR FILE
typedef struct
{
	Elf_Header h;
	SecHead s;
	SymTab st;
	StringTab string1;
	StringTab string2;
	ListReimpTab LR;
	FILE *f;
} OFile;



// PARTIE 1.1
Elf_Header read_header(FILE * f);
void print_header(Elf_Header h);

// PARTIE 1.2
SecHead read_section_headers(FILE * f, Elf_Header h);
void print_section_headers(SecHead s, StringTab string);

// STRING TAB
StringTab read_string_table(FILE * f, Elf_Header head, SecHead s, int nb);
//void print_string_tab(StringTab string);
void print_string(StringTab string, int pos);

// PARTIE 1.4
SymTab read_table_symboles(FILE * f, SecHead st, Elf_Header h);
void print_table_symboles(SymTab st, StringTab string);

// PARTIE 1.5
ReimpTab read_table_reimplantation(FILE * f, SecHead s, SymTab st, Elf_Header h, int nb);
void print_table_reimp(ReimpTab r, StringTab string2, StringTab string1);
	// NEW
ListReimpTab read_table_reimplantation_new(FILE * f, SecHead s, SymTab st, Elf_Header h);
void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1);



// UTILS
OFile initOFile(FILE * fich_o);
void printOFile(OFile a);
void end(OFile a);
void print_section(FILE * f, SecHead secHead, int i);


#endif

