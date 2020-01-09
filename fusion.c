#include <stdio.h>

#include "fusion.h"

// FONCTIONS DE FUSION

void fusion(OFile a, OFile b, OFile dest)
{
	// COPIE
	dest.h = a.h;
	dest.s = a.s;

	// HEADER
	printf("\nStep 1 : Write header\n\n");
	write_header(dest);



	/***** OPERATIONS DE FUSION *****/

		// FUSION TABLE SYMBOLE
		printf("Step 1.1 : FUSION DE LA TABLE DES SYMBOLES\n");
		fusion_table_symbole(a, b, dest);


		// FUSION TABLE SYMBOLE
		printf("Step 1.2 : FUSION DE LA TABLE DES REIMPLEMENTATIONS\n\n");
		fusion_table_reimplementation(a, b, dest);



	/***** ECRITURE DES SECTIONS *****/

		printf("Step 2 : Write sections\n");

		int nbSection = (dest.h.architecture == 1) ? dest.h.headformat.e_shnum : dest.h.headformat.e_shnum;
		int currentOffset = dest.h.headformat.e_ehsize;

		OneHeader *currentSecHead = dest.s.tete;
		OneHeader *headerOfB = b.s.tete;

		int offset_temporaire;


		// On parcourt toutes les sections Headers

		for (int i = 0; i < nbSection; i++)
		{
			printf("\t%d/%d\n", i + 1, nbSection);

			offset_temporaire = currentOffset;
			currentOffset += currentSecHead->tableformat.sh_size;

			write_section(a, dest, currentSecHead);


			// SEARCH FOR SAME SECTION IN FILE B

			while (headerOfB->tableformat.sh_name != currentSecHead->tableformat.sh_name && headerOfB->suivant != NULL)
			{
				headerOfB = headerOfB->suivant;
			}

			if (headerOfB->tableformat.sh_name == currentSecHead->tableformat.sh_name 
				&& currentSecHead->tableformat.sh_type != SHT_SYMTAB && currentSecHead->tableformat.sh_type != SHT_RELA)
			{
				write_section(b, dest, headerOfB);
				currentOffset += headerOfB->tableformat.sh_size;
				currentSecHead->tableformat.sh_size += headerOfB->tableformat.sh_size;
			}

			headerOfB = b.s.tete;
			// END SEARCH

			currentSecHead->tableformat.sh_offset = offset_temporaire;
			currentSecHead = currentSecHead->suivant;
		}



	/***** ECRITURE SECTION HEADER *****/

		printf("\nStep 3 : Write section header table\n\n");
		write_section_header(dest);



	/***** MAJ *****/

		printf("Step 4 : MAJ\n\n");
		dest.h.headformat.e_shoff = currentOffset;

		int offset = dest.h.architecture == 1 ? 0x20 : 0x28;
		fseek(dest.f, offset, SEEK_SET);

		if (dest.h.architecture == 1)
		{
			write_word(dest.h.headformat.e_shoff, dest.f, dest.h.endianess);
		}
		else
		{
			write_double_word(dest.h.headformat.e_shoff, dest.f, dest.h.endianess);
		}

	


	printf("Fusion FINISHED\n\n");
}



// FONCTIONS D'ECRITURE

void write_header(OFile dest)
{
	for (int i = 0; i < 16; i++)
	{
		write_quarter_word(dest.h.headformat.e_ident[i], dest.f);
	}

	write_half_word(dest.h.headformat.e_type, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_machine, dest.f, dest.h.endianess);
	write_word(dest.h.headformat.e_version, dest.f, dest.h.endianess);

	if (dest.h.architecture == 1)
	{
		write_word(dest.h.headformat.e_entry, dest.f, dest.h.endianess);
		write_word(dest.h.headformat.e_phoff, dest.f, dest.h.endianess);
		write_word(dest.h.headformat.e_shoff, dest.f, dest.h.endianess);
	}
	else if (dest.h.architecture == 2)
	{
		write_double_word(dest.h.headformat.e_entry, dest.f, dest.h.endianess);
		write_double_word(dest.h.headformat.e_phoff, dest.f, dest.h.endianess);
		write_double_word(dest.h.headformat.e_shoff, dest.f, dest.h.endianess);
	}

	write_word(dest.h.headformat.e_flags, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_ehsize, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_phentsize, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_phnum, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_shentsize, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_shnum, dest.f, dest.h.endianess);
	write_half_word(dest.h.headformat.e_shstrndx, dest.f, dest.h.endianess);
}

void write_section_header(OFile dest)
{
	int nbSection = (dest.h.architecture == 1) ? dest.h.headformat.e_shnum : dest.h.headformat.e_shnum;

	OneHeader *currentSecHead = dest.s.tete;

	for (int i = 0; i < nbSection; i++)
	{
		write_word(currentSecHead->tableformat.sh_name, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_type, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_flags, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_addr, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_offset, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_size, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_link, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_info, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_addralign, dest.f, dest.h.endianess);
		write_word(currentSecHead->tableformat.sh_entsize, dest.f, dest.h.endianess);

		currentSecHead = currentSecHead->suivant;
	}
}

void write_section(OFile source, OFile dest, OneHeader *secHeader)
{
	char c;
	fseek(source.f, secHeader->tableformat.sh_offset, SEEK_SET);

	for (int j = 0; j < secHeader->tableformat.sh_size; j++)
	{
		c = read_unsigned_char(source.f);
		write_quarter_word(c, dest.f);
	}
}



// FONCTIONS DE FUSION DE LA TABLE DES SYMBOLES

void fusion_table_symbole(OFile a, OFile b, OFile dest)
{
	SymTab symTab1, symTab2;

	symTab1 = a.st;
	symTab2 = b.st;

	// GET LAST SYMBOL
	OneSymbol *endDestSymTab = symTab1.tete;
	while (endDestSymTab->suivant != NULL)
		endDestSymTab = endDestSymTab->suivant;


	// ECRITURE DE TOUS LES SYMBOLES DE SYMTAB2 DANS SYMTAB1
	OneSymbol *currentB = symTab2.tete;
	OneSymbol *previous = symTab2.tete;

	for (int i = 0; i < symTab2.nb; i++)
	{
		printf("\t%d/%d", i + 1, symTab2.nb);

		if( comparaison(symTab1, currentB, previous, endDestSymTab, a.h.architecture) )
		{
			symTab1.nb += 1;
			endDestSymTab = endDestSymTab->suivant;
		}
		currentB = currentB->suivant;
	}
	endDestSymTab->suivant = NULL;
	

	// MODIFICATION DU NOMBRE DE SYMBOLES
	OneHeader *headerA = a.s.tete;
	while(headerA->tableformat.sh_type != SHT_SYMTAB && headerA->suivant != NULL)
		headerA = headerA->suivant;

	headerA->tableformat.sh_info = symTab1.nb;

	printf("\n\n");
}

int comparaison(SymTab symTab1, OneSymbol *current, OneSymbol *previous, OneSymbol *endDestSymTab, int architecture)
{
	/***** SYMBOLE LOCAL, ON L'AJOUTE *****/

		if (ELF32_ST_BIND(current->tableformat.st_value) == STB_LOCAL)
		{
			
			printf("\tLOC\n");

			OneSymbol *ajout;

			ajout = malloc(sizeof(OneSymbol));
			ajout->tableformat = current->tableformat;
			ajout->suivant = NULL;

			endDestSymTab->suivant = ajout;
			
			return 1;
		}



	/***** SYMBOLE GLOBAL, VERIFICATIONS *****/

		else if (ELF32_ST_BIND(current->tableformat.st_value) == STB_GLOBAL)
		{
			printf("\tGLOB");

			OneSymbol *currentts1 = symTab1.tete;

			//On regarde tous les symboles
			for (int i = 0; i < symTab1.nb; i++)
			{
				if (current->tableformat.st_name == currentts1->tableformat.st_name)
				{
					printf("\tC1");
					printf("\tDeux Globeaux ont le même nom , impossible a fusionner\n");
					exit(1);
				}

				else if ((current->tableformat.st_shndx == SHN_UNDEF && currentts1->tableformat.st_shndx != SHN_UNDEF))
				{
					printf("\tC2\n");
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

				else if ((current->tableformat.st_shndx != SHN_UNDEF && currentts1->tableformat.st_shndx == SHN_UNDEF))
				{
					printf("\tC3");
					printf("\tSymbole defini dans fichier 1, on laisse la définition\n");
					return 0;
				}

				else if (current->tableformat.st_shndx == SHN_UNDEF && currentts1->tableformat.st_shndx == SHN_UNDEF)
				{
					printf("\tC4");
					printf("\tDouble Non Def, on laisse qu'une seule non def\n");
					return 0;
				}
				currentts1 = currentts1->suivant;
			}
			printf("\tEnd FOR\n");
			endDestSymTab->suivant = current;
			return 1;
		}



	/***** NI GLOBAL NI LOCAL *****/

		else if (ELF32_ST_BIND(current->tableformat.st_value) == STB_WEAK)
		{
			printf("\tWEAK\n");
			return 0;
		}

		else if (ELF32_ST_BIND(current->tableformat.st_value) == STB_LOPROC)
		{
			printf("\tLOPROC\n");
			return 0;
		}

		else if (ELF32_ST_BIND(current->tableformat.st_value) == STB_HIPROC)
		{
			printf("\tHIPROC\n");
			return 0;
		}

		printf("\tERROR");
		printf("\t%d\n", ELF32_ST_BIND(current->tableformat.st_value) );
		return 0;
}


// FONCTIONS DE FUSION DES TABLES DE REIMPLEMENTATIONS

void fusion_reimp(ReimpTab reimpA, ReimpTab reimpB)
{
	// GET LAST REIMP
	OneReimp * endReimp = reimpA.tete;
	while (endReimp->suivant != NULL)
		endReimp = endReimp->suivant;


	// ECRITURE DE TOUS LES REIMPLEMENTATIONS DE REIMPB DANS REIMPA
	OneReimp *currentB = reimpB.tete;

	for (int i=0 ; i < reimpB.nb ; i++)
	{
		printf("\t\t%d/%d\n", i + 1, reimpB.nb);
		currentB = currentB->suivant;
	}
	endReimp->suivant = NULL;
	

	// MODIFICATIONS
	printf("\n");
}

void fusion_table_reimplementation(OFile a, OFile b, OFile dest)
{
	OneList *reimpA = a.LR.tete;
	OneList *reimpB = b.LR.tete;

	int nb;
	if (a.LR.nb > b.LR.nb) nb = a.LR.nb;
	else nb = b.LR.nb; 

	for (int i=0 ; i < nb ; i++)
	{
		printf("\tFUSION DE LA TABLE %d\n", i);

		fusion_reimp(reimpA->r, reimpB->r);
		reimpA = reimpA->suivant;
		reimpB = reimpB->suivant;

		i++;
	}
}