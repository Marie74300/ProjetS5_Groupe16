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
		while(headerOfB->t32.sh_name != currentSecHead->t32.sh_name && headerOfB->suivant != NULL)
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

	if(dest.h.architecture == 1)
	{
		write_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}
	else
	{
		write_double_word(dest.h.h32.e_shoff, dest.f, dest.h.endianess);
	}

	printf("Fusion FINISHED\n\n");
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
	fseek(source.f, secHeader->t32.sh_offset ,SEEK_SET);

	for(int j=0 ; j < secHeader->t32.sh_size; j++)
	{
		c = read_unsigned_char(source.f);
		write_quarter_word(c, dest.f);
	}
}
