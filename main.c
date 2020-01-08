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
	

	// READING OF THE ELF HEADER
	Elf32_Ehdr head = read_header(fich_o);
	print_header(head);

	// READING OF SECTION HEADERS
	SecHead sec;
	sec.nb = head.e_shnum;
	read_section_headers(fich_o, head, sec);
 
	//AFFICHER SECTION
	print_section(fich_o, 1);

	//AFFICHER TABLE SYMBOLES
	read_table_symboles(fich_o, head);



	fclose(fich_o);
	return 0;
}

