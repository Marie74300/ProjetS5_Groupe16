#ifndef __ES_H__
#define __ES_H__

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

//READ

/*
   read_Elf32_Addr
   description : lis element taille 4 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf32_Addr (FILE *f, int endianess);

/*
   read_Elf64_Addr
   description : lis element taille 8 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf64_Addr (FILE *f, int endianess);

/*
   read_Elf32_Half
   description : lis element taille 2 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf32_Half (FILE *f, int endianess);

/*
   read_Elf32_Off
   description : lis element taille 4 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf32_Off (FILE *f, int endianess);

/*
   read_Elf64_Off
   description : lis element taille 8 du fichier FILE sur 64 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf64_Off (FILE *f, int endianess);

/*
   read_Elf32_Sword
   description : lis element taille 4 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf32_Sword (FILE *f, int endianess);

/*
   read_Elf32_Word
   description : lis element taille 4 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf32_Word (FILE *f, int endianess);

/*
   read_Elf64_Word
   description : lis element taille 8 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_Elf64_Word (FILE *f, int endianess);

/*
   read_unsigned_char
   description : lis element taille 1 du fichier FILE sur 32 bit
   parametres : un fichier et l'endianess du fichier
   valeur de retour : l'élément lu
   effets de bord : aucun
*/
int read_unsigned_char (FILE *f);


// WRITE
/*
   write_quarter_word
   description : écris element de taille 8 dans le fichier f
   parametres : un élément c et le fichier d'écriture f
   valeur de retour : aucun
   effets de bord : modifie le fichier f
*/
void write_quarter_word (int c, FILE *f);
/*
   write_half_word
   description : écris element de taille 16 dans le fichier f
   parametres : un élément c et le fichier d'écriture f
   valeur de retour : aucun
   effets de bord : modifie le fichier f
*/
void write_half_word (int c, FILE *f, int endianess);
/*
   write_word
   description : écris element de taille 32 dans le fichier f
   parametres : un élément c et le fichier d'écriture f
   valeur de retour : aucun
   effets de bord : modifie le fichier f
*/
void write_word (int c, FILE *f, int endianess);
/*
   write_double_word
   description : écris element de taille 64 dans le fichier f
   parametres : un élément c et le fichier d'écriture f
   valeur de retour : aucun
   effets de bord : modifie le fichier f
*/
void write_double_word (int c, FILE *f, int endianess);

#endif
