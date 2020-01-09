#include <stdio.h>
#include <elf.h>
#include <stdlib.h>

#include "read_func.h"

int main(int argc, char ** argv)
{
	if (argc != 3) {
		printf("2 argument attendu. ./partie1 fichier.o sectionNumber\n");
		return 1;
	}
	FILE *fich_o = fopen (argv[1], "r");
	if (fich_o == NULL){
		printf("Erreur ouverture en lecture.\n");
		return 2;
	}

	int sectionNumber = atoi(argv[2]);

	OFile a = initOFile(fich_o);
	printOFile(a, sectionNumber);


	end(a);
	fclose(fich_o);
	return 0;
}

