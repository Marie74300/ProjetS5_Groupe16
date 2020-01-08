#include <stdio.h>
#include <elf.h>
#include "utils.h"
#include "fusion.h"

int main(int argc, char ** argv)
{
	if (argc != 4) {
		printf("3 arguments attendus.\n");
		return 1;
	}
	FILE *fich_o1 = fopen (argv[1], "r");
	if (fich_o1 == NULL){
		printf("Erreur ouverture en lecture 1.\n");
		return 2;
	}
	FILE *fich_o2 = fopen (argv[2], "r");
	if (fich_o2 == NULL){
		printf("Erreur ouverture en lecture 2.\n");
		return 3;
	}
	FILE *dest = fopen (argv[3], "w");
	if (dest == NULL){
		printf("Erreur ouverture en ecriture.\n");
		return 4;
	}

	// 1er FICHIER .o
	OFile a;
	a.h = read_header(fich_o1);
	a.s = read_section_headers(fich_o1, a.h);
	a.string1 = read_string_table(fich_o1, a.h, a.s, 1);
	a.string2 = read_string_table(fich_o1, a.h, a.s, 2);
	a.st = read_table_symboles(fich_o1, a.s);
	a.r = read_table_reimplantation(fich_o1, a.s, a.st);

	// 2eme FICHIER .o
	OFile b;
	b.h = read_header(fich_o2);
	b.s = read_section_headers(fich_o2, b.h);
	b.string1 = read_string_table(fich_o2, b.h, b.s, 1);
	b.string2 = read_string_table(fich_o2, b.h, b.s, 2);
	b.st = read_table_symboles(fich_o2, b.s);
	b.r = read_table_reimplantation(fich_o2, b.s, b.st);

	// FUSION
	fusion (a, b, dest);
	




	fclose(fich_o1);
	fclose(fich_o2);
	fclose(dest);
	return 0;
}

