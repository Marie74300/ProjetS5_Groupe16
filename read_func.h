#ifndef __UTILS_H__
#define __UTILS_H__


// POWER
int power(int entier1, int enier2);
//calcul la puissance


// STRUCTURE POUR HEADER
typedef struct
{
	Elf64_Ehdr headformat;
	int architecture;
	int endianess;
	int elf;
} Elf_Header;


// STRUCTURES POUR SECTIONS HEADER
typedef struct _OneHeader OneHeader;
struct _OneHeader
{
	Elf64_Shdr tableformat;
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
	Elf64_Sym tableformat;
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
	Elf64_Rel tableformat;
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
	char name[26];
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
/*
   read_header
   description : Stock les données du header du fichier f.
   paramètres  : Fichier f ouvert préalablement.
   valeur de retour : variable head ("contenant toute les informations du header du fichier) avec la structure Elf_Header.
   effets de bord : Remplissage des partie de Elf_header soit  :-e_type grâce à read_Elf32_Half
   								-e_machine grâce à read_Elf32_Half
   								-e_version grâce à read_Elf32_Word
   								-e_machine grâce à read_Elf32_Half
   								-e_entry grâce à read_Elf32_Adrr (ou read_Elf64_Adrr)
   								-e_phoff grâce à read_Elf32_Off (ou read_Elf64_Off) 	
   								-e_shoff grâce à read_Elf32_Off (ou read_Elf64_Off) 
   								-e_flash grâce à read_Elf32_Word (ou read_Elf32_Off)
   								-...
   		

*/

// PARTIE 1.2
SecHead read_section_headers(FILE * f, Elf_Header head);
/****************************************************************************************************************************************
   read_section_header
   description :  insetion des sections en ce plaçant dans la partie conserné du fichier F grâce au données du header (head) .
   paramètres  : Fichier f ouvert préalablement Elf_header head remplis dans ead_header.
   valeur de retour : variable  ("contenant toute les informations du header du fichier) avec la structure Elf_Header.
   effets de bord : Remplissage des partie de Elf_header soit  :-sh_type grâce à read_Elf32_Half
   								-sh_machine grâce à read_Elf32_Half
   								-sh_version grâce à read_Elf32_Word
   								-sh_machine grâce à read_Elf32_Half
   								-sh_entry grâce à read_Elf32_Adrr (ou read_Elf64_Adrr)
   								-sh_phoff grâce à read_Elf32_Off (ou read_Elf64_Off) 	
   								-sh_shoff grâce à read_Elf32_Off (ou read_Elf64_Off) 
   								-sh_flash grâce à read_Elf32_Word (ou read_Elf32_Off)
   								-...
   		

****************************************************************************************************************************************/


// STRING TAB
StringTab read_string_table(FILE * f, Elf_Header head, SecHead section, int nb);


// PARTIE 1.4
SymTab read_table_symboles(FILE * f, SecHead symbole_table, Elf_Header head);


// PARTIE 1.5
ReimpTab read_table_reimplantation(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head, int nb);


ListReimpTab read_table_reimplantation_new(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head);



// UTILS
OFile initOFile(FILE * fich_o);
void printOFile(OFile a);
void end(OFile a);
void print_section(FILE * f, SecHead secHead, int i);


#endif

