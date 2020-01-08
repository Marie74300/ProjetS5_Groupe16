#include <stdio.h>
#include <elf.h>
#include "utils.h"

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


	/*Elf_Header h = read_header(fich_o);
	print_header (h);
	
	SecHead s = read_section_headers(fich_o, h);

	for (int i=0 ; i<s.nb ; i++)
		print_section(fich_o, s, i);

	StringTab string1 = read_string_table(fich_o, h, s, 0);
	print_section_headers(s, string1);*/


	end(a);
	fclose(fich_o);
	return 0;
}

