#include "fusion.h"


// FONCTIONS DE FUSION

void fusion(OFile a, OFile b, OFile dest)
{
	// COPIE
	dest.h = a.h;
	dest.s = a.s;

	// HEADER
	printf("write header\n");
	write_header(dest);
	

	// SECTIONS
	printf("write sections\n");
	// Nombre de sections
	int nbSection = dest.h.architecture == 1 ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;

	//int compt = 0;
	//int pcur = dest.h.architecture ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;
	//int min = 100000, pmin = 0;
	//int name, c2_compt;

	// int tab[nbSection];
	// for(int i=0 ; i<nbSection ; i++)
	// 	tab[i] = 0;

	// Indice des sections
	//int e_shnum2 = b.h.architecture ? b.h.h32.e_shnum : b.h.h64.e_shnum;
	//int tab2[e_shnum2];
	// for(int i=0 ; i<e_shnum2 ; i++)
	// 	tab2[i] = 0;


	int currentOffset = dest.h.h32.e_ehsize;

	OneHeader *currentSecHead = dest.s.tete;
	//OneHeader *c2 = b.s.tete;

	// On parcourt toutes les sections Headers
	for (int i = 0; i < nbSection; i++) {
		if(currentSecHead->t32.sh_offset != 0){
			//Maj secHead
			currentSecHead->t32.sh_offset = currentOffset;
			currentOffset += currentSecHead->t32.sh_size;
		}

		printf("section %d/%d\n", i + 1, nbSection);
		write_section(a, dest, currentSecHead);
		currentSecHead = currentSecHead->suivant;
	}

	

	// while (compt != nbSection)
	// {
	// 	for(int i=0 ; i < nbSection ; i++){
	// 		if (c->t32.sh_offset < min && tab[i] == 0)
	// 		{
	// 			min = c->t32.sh_offset;
	// 			pmin = i;
	// 			name = c->t32.sh_name;
	// 		}
	// 		c = c->suivant;
	// 	}
	// 	while (min > pcur)
	// 	{
	// 		write_quarter_word(0, dest.f);
	// 		pcur++;
	// 	}
	// 	write_section(a, dest, pmin);
	//
	// 	// CHERCHER SECTION DANS 2EME FICHIER .O
	//
	// 	c2 = b.s.tete;
	// 	c2_compt = 0;
	// 	while (c2->t32.sh_name != name && c2->suivant != NULL)
	// 	{
	// 		c2 = c2->suivant;
	// 		c2_compt++;
	// 	}
	//
	// 	if (c2->t32.sh_name == name)
	// 	{
	// 		write_section(b, dest, c2_compt);
	// 		//tab2[c2_compt] = 1;
	// 	}
	//
	// 	// FIN RECHERCHE
	//
	// 	compt++;
	// 	tab[pmin] = 1;
	// 	min = 100000;
	// 	pmin = 0;
	// 	c = a.s.tete;
	// }



	// ECRITURE DES SECTIONS UNIQUES DANS B
	// c2 = b.s.tete;
	// c = dest.s.tete;
	//
	//
	// OneHeader *suiv = malloc (sizeof(OneHeader));
	// suiv->suivant = NULL;
	//
	// while (c->suivant != NULL)
	// 	c = c->suivant;
	//
	// for(int i=0 ; i<e_shnum2 ; i++)
	// {
	// 	if (tab2[i] == 0)
	// 	{
	// 		write_section(b, dest, i);
	// 	}
	//
	// 	c2 = c2->suivant;
	// }



	// ECRITURE SECTION HEADER
	printf("write section header table\n");
	write_section_header(dest);



	//Maj HEADER
	dest.h.h32.e_shoff = currentOffset;

 	int offset = dest.h.architecture == 1 ? 0x20 : 0x28;
	fseek(dest.f, offset, SEEK_SET);

	if(dest.h.architecture == 1)
	{
		write_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}
	else
	{
		write_double_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}

	printf("Fusion OK\n");
}


// FONCTIONS D'ECRITURE

void write_header(OFile dest)
{
	for (int i=0 ; i<16 ; i++)
	{
		write_quarter_word(dest.h.h32.e_ident[i], dest.f);
	}

	if(dest.h.architecture==1)
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
	else if(dest.h.architecture==2)
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

	for(int i=0; i < nbSection; i++)
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

//write the section of the given section header of the source file in the dest file
void write_section(OFile source, OFile dest, OneHeader *secHeader)
{
	char c;
	// printf("section decal [%x]\n", secHeader->t32.sh_offset);
	// printf("section size [%x]\n", secHeader->t32.sh_size);
	fseek(source.f, secHeader->t32.sh_offset ,SEEK_SET);
	for(int j=0 ; j < secHeader->t32.sh_size; j++)
	{
		c = read_unsigned_char(source.f);
		write_quarter_word(c, dest.f);
	}
}
