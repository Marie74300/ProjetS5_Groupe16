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


	OFile a;

	// READING OF THE ELF HEADER
	a.h = read_header(fich_o);
	print_header(a.h);

	// READING OF SECTION HEADERS
	a.s = read_section_headers(fich_o, a.h);

		// READING OF STRING TABLE
		a.string1 = read_string_table(fich_o, a.h, a.s, 1);
		//print_string_tab(a.string1);

		a.string2 = read_string_table(fich_o, a.h, a.s, 2);
		//print_string_tab(a.string2);

	print_section_headers(a.s, a.string1);



	//AFFICHER SECTION
	print_section(fich_o, a.s, 3);

	//AFFICHER TABLE SYMBOLES
	a.st = read_table_symboles(fich_o, a.s, a.h);
	print_table_symboles(a.st, a.string2);

	//AFFICHER Table de réimplantation
	a.r = read_table_reimplantation(fich_o, a.s, a.st, a.h);
	print_table_reimp(a.r, a.string2, a.string1);

	




	end(a);
	fclose(fich_o);
	return 0;
}

