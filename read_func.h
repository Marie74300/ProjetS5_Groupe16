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
/****************************************************************************************************************************************
   read_header
   description : Stock les donnees du header du fichier f.
   parametres  : Fichier f ouvert prealablement.
   valeur de retour : variable head ("contenant toute les informations du header du fichier) avec la structure Elf_Header.
   effets de bord : Remplissage des partie de Elf_header soit  :-e_type grace a read_Elf32_Half
   								-e_machine grace a read_Elf32_Half
   								-e_version grace a read_Elf32_Word
   								-e_machine grace a read_Elf32_Half
   								-e_entry grace a read_Elf32_Adrr (ou read_Elf64_Adrr)
   								-e_phoff grace a read_Elf32_Off (ou read_Elf64_Off) 	
   								-e_shoff grace a read_Elf32_Off (ou read_Elf64_Off) 
   								-e_flash grace a read_Elf32_Word (ou read_Elf32_Off)
   								-...
   		

****************************************************************************************************************************************/
Elf_Header read_header(FILE * f);


// PARTIE 1.2
/****************************************************************************************************************************************
   read_section_header
   description :  insetion des sections en ce placant dans la partie conserne du fichier F grace au donnees du header (head) .
   parametres  : Fichier f ouvert prealablement Elf_header head remplis dans ead_header.
   valeur de retour : variable  ("contenant toute les informations du header du fichier) avec la structure Elf_Header.
   effets de bord : Remplissage des partie de Elf_header soit  :-sh_type grace a read_Elf32_Half
   								-sh_machine grace a read_Elf32_Half
   								-sh_version grace a read_Elf32_Word
   								-sh_machine grace a read_Elf32_Half
   								-sh_entry grace a read_Elf32_Adrr (ou read_Elf64_Adrr)
   								-sh_phoff grace a read_Elf32_Off (ou read_Elf64_Off) 	
   								-sh_shoff grace a read_Elf32_Off (ou read_Elf64_Off) 
   								-sh_flash grace a read_Elf32_Word (ou read_Elf32_Off)
   								-...
   		

****************************************************************************************************************************************/
SecHead read_section_headers(FILE * f, Elf_Header head);



// STRING TAB
/****************************************************************************************************************************************
   read_string_table
   description :  insetion des string.
   param�tres  : Fichier f ouvert prealablement Elf_header head SecHead section int nb (1 ou 2)
   valeur de retour : variable  sting table
   effets de bord : Remplissage des Strings 
   		

****************************************************************************************************************************************/
StringTab read_string_table(FILE * f, Elf_Header head, SecHead section, int nb);



// PARTIE 1.4
/****************************************************************************************************************************************
   read_table_symboles
   description :  insetion des symboles celon la header et le fichier
   parametres  : Fichier f ouvert pr�alablement Elf_header head remplis dans read_header.
   valeur de retour : variable SymTab
   effets de bord : Remplissages des Table symboles dans les variable mis en place pour.
****************************************************************************************************************************************/
SymTab read_table_symboles(FILE * f, SecHead symbole_table, Elf_Header head);



// PARTIE 1.5
/****************************************************************************************************************************************
   read_table_symboles
   description :  insetion des table de reimplantation
   parametres  : Fichier f ouvert pr�alablement Elf_header head remplis dans read_header int nb section.
   valeur de retour : ReimpTab
   effets de bord : Remplissages des Table reimplantation dans les variable mis en place pour.
****************************************************************************************************************************************/
ReimpTab read_table_reimplantation(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head, int nb);


/****************************************************************************************************************************************
   read_stable_reimplantation_new
   description :  appel read_table_reimplantation pour pouvoir traiter table par table.
   param�tres  : Fichier f ouvert prealablement Elf_header head remplis dans ead_header SymTab symbole_table remplis .
   valeur de retour : variable ListReimpTab contient une liste de ReimpTab
   effets de bord : Remplissage de la liste de ReimpTab 
   		

****************************************************************************************************************************************/
ListReimpTab read_table_reimplantation_new(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head);





// UTILS
OFile initOFile(FILE * fich_o);
void printOFile(OFile a, int sectionNumber);
void end(OFile a);
void print_section(FILE * f, SecHead secHead, int i);


#endif

