#ifndef __READ_FUNC_H__
#define __READ_FUNC_H__


// POWER
int power(int entier1, int enier2);
//calcul la puissance


// STRUCTURE POUR HEADER
typedef struct
{
	Elf64_Ehdr headformat;
	int architecture;
	int endianess;
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
/****************************************************************************************************************************************
   read_string_table
   description :  insetion des string.
   paramètres  : Fichier f ouvert préalablement Elf_header head SecHead section int nb (1 ou 2)
   valeur de retour : variable  sting table
   effets de bord : Remplissage des Strings 
   		

****************************************************************************************************************************************/


// PARTIE 1.4
SymTab read_table_symboles(FILE * f, SecHead symbole_table, Elf_Header head);
/****************************************************************************************************************************************
   read_table_symboles
   description :  insetion des symboles celon la header et le fichier
   paramètres  : Fichier f ouvert préalablement Elf_header head remplis dans read_header.
   valeur de retour : variable SymTab
   effets de bord : Remplissages des Table symboles dans les variable mis en place pour.
****************************************************************************************************************************************/


// PARTIE 1.5
ReimpTab read_table_reimplantation(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head, int nb);
/****************************************************************************************************************************************
   read_table_symboles
   description :  insetion des table de reimplantation
   paramètres  : Fichier f ouvert préalablement Elf_header head remplis dans read_header int nb section.
   valeur de retour : ReimpTab
   effets de bord : Remplissages des Table reimplantation dans les variable mis en place pour.
****************************************************************************************************************************************/

ListReimpTab read_table_reimplantation_new(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head);
/****************************************************************************************************************************************
   read_stable_reimplantation_new
   description :  appel read_table_reimplantation pour pouvoir traiter table par table.
   paramètres  : Fichier f ouvert préalablement Elf_header head remplis dans ead_header SymTab symbole_table remplis .
   valeur de retour : variable ListReimpTab contient une liste de ReimpTab
   effets de bord : Remplissage de la liste de ReimpTab 
   		

****************************************************************************************************************************************/




// UTILS
OFile initOFile(FILE * fich_o);
void printOFile(OFile a);
void end(OFile a);
void print_section(FILE * f, SecHead secHead, int i);


#endif

