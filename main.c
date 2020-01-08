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
	SecHead secHead;
	secHead.nb = head.e_shnum;
	secHead = read_section_headers(fich_o, head, secHead);

	//AFFICHER SECTION
	print_section(fich_o, secHead, 3);

	//AFFICHER TABLE SYMBOLES
	SymTab symTab;
	symTab = read_table_symboles(fich_o, secHead, symTab);
	print_table_symboles(symTab);

	//AFFICHER Table de réimplantation
	read_table_reimplantation(fich_o, secHead, symTab);

	// Read string TABLE TODO
	print_string_table(head, secHead);

	fclose(fich_o);
	return 0;
}
