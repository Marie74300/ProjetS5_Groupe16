#ifndef __UTILS_H__
#define __UTILS_H__


/*
   power
   description : calcul la puissance
   parametres : deux entiers dont le second correspond à la puissance
   valeur de retour : l'entier calculé
   effets de bord : aucun
*/
int power(int a, int b);


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
/*
   read_header
   description : lis l'entete du fichier donné en paramètre
   parametres : fichier au format ELF
   valeur de retour : un header rempli avec le header du fichier
   effets de bord : aucun
*/
Elf_Header read_header(FILE * f);

/*
   print_header
   description : affiche le header
   parametres : un header non vide
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_header(Elf_Header h);

// PARTIE 1.2

/*
   read_section_headers
   description : lis l'entete des sections du fichier donné en paramètre
   parametres : fichier au format ELF et un header non vide
   valeur de retour : un section header rempli avec le header de la table des sections du fichier en paramètre
   effets de bord : aucun
*/
SecHead read_section_headers(FILE * f, Elf_Header h);

/*
   print_section_headers
   description : affiche le header de la table des sections
   parametres : un section header non vide
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_section_headers(SecHead s, StringTab string);

/*
   read_string_table
   description : lis la table de string du fichier donné en paramètre
   parametres : fichier au format ELF, son header, sa table des sections et le numéro de la table de string
   valeur de retour : une table de string rempli avec celle du fichier en paramètre
   effets de bord : aucun
*/
StringTab read_string_table(FILE * f, Elf_Header head, SecHead s, int nb);

/*
   print_string
   description : affiche la table de string à parir de la position donnée
   parametres : une table de string et une position
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_string(StringTab string, int pos);

// PARTIE 1.3
/*
   print_section
   description : affiche le contenu d'une section du fichier en paramètre
   parametres : un fichier au format ELF, sa table de section et le numéro de la section à afficher
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_section(FILE * f, SecHead secHead, int i);

// PARTIE 1.4
/*
   read_table_symboles
   description : lis la table des symboles d'un fichier donné en paramètre
   parametres : un fichier au format ELF, l'header de sa table des sections et son header
   valeur de retour : une table des symboles rempli avec celle du fichier donné en paramètre
   effets de bord : aucun
*/
SymTab read_table_symboles(FILE * f, SecHead st, Elf_Header h);

/*
   print_table_symboles
   description : affiche la table des symboles
   parametres : une table de symbole et une table de string
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_table_symboles(SymTab st, StringTab string);

// PARTIE 1.5
/*
   read_table_reimplantation
   description : lis la table de réimplantation du fichier donné en paramètre
   parametres : un fichier au format ELF, le header de ses sections, sa table de symbole, son header et le numéro de la table
   valeur de retour : aucun
   effets de bord : aucun
*/
ReimpTab read_table_reimplantation(FILE * f, SecHead s, SymTab st, Elf_Header h, int nb);

/*
   print_table_reimp
   description : affiche la table de reimplantation
   parametres : une table de reimplantation et deux tables de strings
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_table_reimp(ReimpTab r, StringTab string2, StringTab string1);

/*
   read_table_reimplantation_new
   description : lis la table de réimplantation du fichier donné en paramètre v2
   parametres : un fichier au format ELF, le header de ses sections, sa table de symbole, son header et le numéro de la table
   valeur de retour : aucun
   effets de bord : aucun
*/
ListReimpTab read_table_reimplantation_new(FILE * f, SecHead s, SymTab st, Elf_Header h);

/*
   print_table_reimp_new
   description : affiche la table de reimplantation v2
   parametres : une table de reimplantation et deux tables de strings
   valeur de retour : aucun
   effets de bord : aucun
*/
void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1);


// UTILS
/*
   initOFile
   description : initialise un Ofile avec le fichier donné en paramètre
   parametres : un fichier au format ELF
   valeur de retour : aucun
   effets de bord : alloue de la mémoire
*/
OFile initOFile(FILE * fich_o);

/*
   printOFile
   description : affiche le contenue de la structure Ofile
   parametres : un Ofile
   valeur de retour : aucun
   effets de bord : aucun
*/
void printOFile(OFile a);

/*
   end
   description : libère la mémoire de la structure Ofile donné en paramètre
   parametres : un Ofile
   valeur de retour : aucun
   effets de bord : libère de la mémoire
*/
void end(OFile a);


#endif
