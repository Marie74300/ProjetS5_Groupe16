#include <stdio.h>
#include <elf.h>

#include "read_func.h"

int main(int argc, char ** argv)
{
	if (argc != 2) {
		printf("1 argument attendu.\n");
		return 1;
	}
	FILE *fich_o = fopen (argv[1], "r");
	if (fich_o == NULL){
		printf("Erreur ouverture en lecture.\n");
		return 2;
	}


	OFile a = initOFile(fich_o);
	printOFile(a);


	end(a);
	fclose(fich_o);
	return 0;
}

