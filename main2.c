#include <stdio.h>
#include <elf.h>

#include "utils.h"
#include "fusion.h"
#include "es.h"

int main(int argc, char ** argv)
{
	// TESTS
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
	FILE *f_dest = fopen (argv[3], "w");
	if (f_dest == NULL){
		printf("Erreur ouverture en ecriture.\n");
		return 4;
	}


	OFile a = initOFile(fich_o1);
	OFile b = initOFile(fich_o2);

	// Res FICHIER .o
	OFile dest;
	dest.f = f_dest;
	
	
	// FUSION
	fusion (a, b, dest);
	
	

	end(a);
	end(b);
	
	fclose(fich_o1);
	fclose(fich_o2);
	fclose(f_dest);


	return 0;
}

