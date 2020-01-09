#include "fusion.h"
#include "utils.h"
#include <stdio.h>
// FONCTIONS DE FUSION

void fusion(OFile a, OFile b, OFile dest)
{
	// COPIE
	dest.h = a.h;
	dest.s = a.s;

	// HEADER
	printf("\nStep 1 : Write header\n\n");
	write_header(dest);

	// SECTIONS
	printf("Step 2 : Write sections\n");

	int nbSection = (dest.h.architecture == 1) ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;
	int currentOffset = dest.h.h32.e_ehsize;

	OneHeader *currentSecHead = dest.s.tete;
	OneHeader *headerOfB = b.s.tete;

	int offset_temporaire;

	// On parcourt toutes les sections Headers
	for (int i = 0; i < nbSection; i++)
	{
		printf("\ts%d/%d\n", i + 1, nbSection);

		offset_temporaire = currentOffset;
		currentOffset += currentSecHead->t32.sh_size;

		write_section(a, dest, currentSecHead);

		// SEARCH FOR SAME SECTION IN FILE B
		while (headerOfB->t32.sh_name != currentSecHead->t32.sh_name && headerOfB->suivant != NULL)
		{
			headerOfB = headerOfB->suivant;
		}

		if (headerOfB->t32.sh_name == currentSecHead->t32.sh_name)
		{
			write_section(b, dest, headerOfB);
			currentOffset += headerOfB->t32.sh_size;
			currentSecHead->t32.sh_size += headerOfB->t32.sh_size;
		}
		headerOfB = b.s.tete;

		// END SEARCH

		currentSecHead->t32.sh_offset = offset_temporaire;

		currentSecHead = currentSecHead->suivant;
	}

	// ECRITURE SECTION HEADER
	printf("\nStep 3 : Write section header table\n\n");
	write_section_header(dest);

	//Maj HEADER
	printf("Step 4 : MAJ\n\n");
	dest.h.h32.e_shoff = currentOffset;

	int offset = dest.h.architecture == 1 ? 0x20 : 0x28;
	fseek(dest.f, offset, SEEK_SET);

	if (dest.h.architecture == 1)
	{
		write_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}
	else
	{
		write_double_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}

	// FUSION TABLE SYMBOLE
	printf("Step 5 : FUSION DE LA TABLE DES SYMBOLES\n");
	fusion_table_symbole(a, b, dest);
	printf("Fusion FINISHED\n\n");
}

// FONCTIONS D'ECRITURE

void write_header(OFile dest)
{
	for (int i = 0; i < 16; i++)
	{
		write_quarter_word(dest.h.h32.e_ident[i], dest.f);
	}

	if (dest.h.architecture == 1)
	{
		write_half_word(dest.h.h32.e_type, dest.f, dest.h.endianess);
		write_half_word(dest.h.h32.e_machine, dest.f, dest.h.endianess);

		write_word(dest.h.h32.e_version, dest.f, dest.h.endianess);

		write_word(dest.h.h32.e_entry, dest.f, dest.h.endianess);
		write_word(dest.h.h32.e_phoff, dest.f, dest.h.endianess);
		write_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);

		write_word(dest.h.h32.e_flags, dest.f, dest.h.endianess);

		write_half_word(dest.h.h32.e_ehsize, dest.f, dest.h.endianess);
		write_half_word(dest.h.h32.e_phentsize, dest.f, dest.h.endianess);

		write_half_word(dest.h.h32.e_phnum, dest.f, dest.h.endianess);
		write_half_word(dest.h.h32.e_shentsize, dest.f, dest.h.endianess);

		write_half_word(dest.h.h32.e_shnum, dest.f, dest.h.endianess);
		write_half_word(dest.h.h32.e_shstrndx, dest.f, dest.h.endianess);
	}
	else if (dest.h.architecture == 2)
	{
		write_half_word(dest.h.h64.e_type, dest.f, dest.h.endianess);
		write_half_word(dest.h.h64.e_machine, dest.f, dest.h.endianess);

		write_word(dest.h.h64.e_version, dest.f, dest.h.endianess);

		write_double_word(dest.h.h64.e_entry, dest.f, dest.h.endianess);
		write_double_word(dest.h.h64.e_phoff, dest.f, dest.h.endianess);
		write_double_word(dest.h.h64.e_shoff, dest.f, dest.h.endianess);
		write_word(dest.h.h64.e_flags, dest.f, dest.h.endianess);

		write_half_word(dest.h.h64.e_ehsize, dest.f, dest.h.endianess);
		write_half_word(dest.h.h64.e_phentsize, dest.f, dest.h.endianess);

		write_half_word(dest.h.h64.e_phnum, dest.f, dest.h.endianess);
		write_half_word(dest.h.h64.e_shentsize, dest.f, dest.h.endianess);

		write_half_word(dest.h.h64.e_shnum, dest.f, dest.h.endianess);
		write_half_word(dest.h.h64.e_shstrndx, dest.f, dest.h.endianess);
	}
}

void write_section_header(OFile dest)
{
	int nbSection = (dest.h.architecture == 1) ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;

	OneHeader *currentSecHead = dest.s.tete;

	for (int i = 0; i < nbSection; i++)
	{
		write_word(currentSecHead->t32.sh_name, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_type, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_flags, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_addr, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_offset, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_size, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_link, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_info, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_addralign, dest.f, dest.h.endianess);
		write_word(currentSecHead->t32.sh_entsize, dest.f, dest.h.endianess);

		currentSecHead = currentSecHead->suivant;
	}
}

void write_section(OFile source, OFile dest, OneHeader *secHeader)
{
	char c;
	fseek(source.f, secHeader->t32.sh_offset, SEEK_SET);

	for (int j = 0; j < secHeader->t32.sh_size; j++)
	{
		c = read_unsigned_char(source.f);
		write_quarter_word(c, dest.f);
	}
}

void fusion_table_symbole(OFile a, OFile b, OFile dest)
{
	SymTab symTab1, symTab2, tsdest;

	symTab1 = a.st;
	symTab2 = b.st;

	// print_table_symboles(symTab1, a.string2);

	tsdest.tete = symTab1.tete;
	tsdest.nb = symTab1.nb;

	//Get last ref
	OneSymbol *endDestSymTab = tsdest.tete;
	while (endDestSymTab->suivant != NULL)
	{
		endDestSymTab = endDestSymTab->suivant;
	}

	// printf("+++++++%x++++++\n", symTab1.tete->t32.st_value);
	//ecrire dans tsdest symTab1 element par element;
	OneSymbol *currentB = symTab2.tete;
	OneSymbol *previous = symTab2.tete;

	for (int i = 0; i < symTab2.nb; i++)
	{
		printf("Table symbole : %d/%d\n", i + 1, symTab2.nb);

		if(comparaison(symTab1, currentB, previous, endDestSymTab, a.h.architecture)){
			//added
			tsdest.nb += 1;
			endDestSymTab = endDestSymTab->suivant;
		}
		currentB = currentB->suivant;
	}
	print_table_symboles(tsdest, a.string2);
	dest.st = tsdest;

	// TODO : Ecrire dest.st dans le fichier
}

int comparaison(SymTab symTab1, OneSymbol *current, OneSymbol *previous, OneSymbol *endDestSymTab, int architecture)
{
	OneSymbol *currentts1 = symTab1.tete;

	if (architecture == 1)
	{
		if (ELF32_ST_BIND(current->t32.st_value) == STB_LOCAL)
		{
			// LOCAL symbol, Add it
			printf("Local Symbol\n");
			endDestSymTab->suivant = current;
			return 1;
		}
		else if (ELF32_ST_BIND(current->t32.st_value) == STB_GLOBAL)
		{
			printf("Global Symbol\n");

			//On regarde tous les symboles
			for (int i = 0; i < symTab1.nb; i++)
			{
				if (current->t32.st_name == currentts1->t32.st_name)
				{
					printf("Deux Globeaux ont le même nom , impossible a fusionner\n");
					exit(1);
					return 0;
				}
				else if ((current->t32.st_shndx == SHN_UNDEF && currentts1->t32.st_shndx != SHN_UNDEF))
				{
					OneSymbol *temp = current->suivant;

					//Def dans currentts1
					if(previous == current) { //Si current == tete
						current = currentts1;
						current->suivant = temp;
					}else{
						previous->suivant = currentts1;
						previous->suivant->suivant = temp;
					}
					return 0;
				}
				else if ((current->t32.st_shndx != SHN_UNDEF && currentts1->t32.st_shndx == SHN_UNDEF))
				{
					//Def dans current, On laisse la définition
					return 0;
				}
				else if (current->t32.st_shndx == SHN_UNDEF && currentts1->t32.st_shndx == SHN_UNDEF)
				{
					//Double Non Def, on laisse qu'une seule non def
					return 0;
				}
				currentts1 = currentts1->suivant;
			}
			printf("End FOR\n");
			endDestSymTab->suivant = current;
			return 1;
		}
	}

	return 0;
	// else
	// {
	// 	if (ELF64_ST_BIND(current->t64.st_value) == STB_LOCAL)
	// 	{
	// 		finB->suivant = current;
	// 		currentts1 = currentts1->suivant;
	// 		return;
	// 	}
	// 	else if (ELF64_ST_BIND(current->t64.st_value) == STB_GLOBAL)
	// 	{
	// 		if (currentts1->t64.st_name == current->t64.st_name)
	// 		{
	// 			printf("deux global ont le même nom , impossible a fusionnés les deux fichiers ");
	// 			exit(1);
	// 		}
	// 		if ((current->t64.st_shndx == SHN_UNDEF && currentts1->t64.st_shndx != SHN_UNDEF))
	// 		{
	// 			//METTRE QUE LA DEFINITION ??
	// 			finB->suivant = current;
	// 			currentts1 = currentts1->suivant;
	// 			return;
	// 		}
	// 		if (current->t64.st_shndx == SHN_UNDEF && currentts1->t64.st_shndx == SHN_UNDEF)
	// 		{
	// 			//mettre une seule entréé pour ce symbole dans la sortie
	// 			finB->suivant = current;
	// 			currentts1 = currentts1->suivant;
	// 			return;
	// 		}
	// 	}
	// }
}
