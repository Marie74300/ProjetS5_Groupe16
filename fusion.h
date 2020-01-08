#ifndef __FUSION_H__
#define __FUSION_H__

#include <stdio.h>
#include <elf.h>
#include "utils.h"

void write_quarter_word (int c, FILE *f);
void write_half_word (int c, FILE *f);
void write_word (int c, FILE *f);
void write_double_word (int c, FILE *f);

void fusion(OFile a, OFile b, OFile dest);
/*
On a vu les choses de la manière suivante:
	- En premier lieu on copie les elements de la structure OFile a dans OFile dest
	- Dans un deuxieme temps on traite la fusion des sections / renommage
	- Dans un troiseme temps on traite les rennomages des symboles
	- Dans un dernier temps on ecrit le contenu de la structure OFile dest dans le fichier dest.l
*/
void write_header(OFile dest);
void write_section(FILE * lec, FILE * ec, SecHead s, int i);
void write_section_header(OFile dest);
#endif

