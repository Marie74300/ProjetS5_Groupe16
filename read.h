#ifndef __READ_H__
#define __READ_H__

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

//READ
//Lis element taille 4 du fichier FILE sur 32 bit
int read_Elf32_Addr (FILE *f, int endianess);

//Lis element taille 8 du fichier FILE sur 64 bit
int read_Elf64_Addr (FILE *f, int endianess);

//Lis element taille 2 du fichier FILE sur 32 bit 
int read_Elf32_Half (FILE *f, int endianess);

//Lis element taille 4 du fichier FILE sur 32 bit
int read_Elf32_Off (FILE *f, int endianess);

//Lis element taille 8 du fichier FILE sur 64 bit
int read_Elf64_Off (FILE *f, int endianess);

//Lis element taille 4 du fichier FILE sur 32 bit
int read_Elf32_Sword (FILE *f, int endianess);

//Lis element taille 4 du fichier FILE sur 32 bit
int read_Elf32_Word (FILE *f, int endianess);

//Lis element taille 8 du fichier FILE sur 64 bit
int read_Elf64_Word (FILE *f, int endianess);

//Lis element taille 1 du fichier FILE sur 32 bit
int read_unsigned_char (FILE *f);



// WRITE

void write_quarter_word (int c, FILE *f);
void write_half_word (int c, FILE *f, int endianess);
void write_word (int c, FILE *f, int endianess);
void write_double_word (int c, FILE *f, int endianess);

#endif
