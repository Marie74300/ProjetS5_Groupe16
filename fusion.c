#include "fusion.h"

void write_quarter_word (int c, FILE *f){fwrite(&c, 1, 1, f);}
void write_half_word (int c, FILE *f){fwrite(&c, 2, 1, f);}
void write_word (int c, FILE *f){fwrite(&c, 4, 1, f);}
void write_double_word (int c, FILE *f){fwrite(&c, 8, 1, f);}


// FONCTIONS DE FUSION

void fusion(OFile a, OFile b, OFile dest)
{



	// COPIE
	dest.h = a.h;
	dest.s = a.s;
	


	// FUSION DES SECTIONS DE A
	write_header(dest);

	int e_shnum = dest.h.architecture ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;
	int compt = 0;
	int pcur = dest.h.architecture ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;
	int min = 100000, pmin = 0;
	int name, c2_compt;

	int e_shnum2 = b.h.architecture ? b.h.h32.e_shnum : b.h.h64.e_shnum;
	int tab2[e_shnum2];
	for(int i=0 ; i<e_shnum2 ; i++)
		tab2[i] = 0;

	OneHeader *c = dest.s.tete;
	OneHeader *c2 = b.s.tete;

	int tab[e_shnum];
	for(int i=0 ; i<e_shnum ; i++)
		tab[i] = 0;

	while (compt != e_shnum)
	{
		for(int i=0 ; i < e_shnum ; i++){
			if (c->t32.sh_offset < min && tab[i] == 0)
			{
				min = c->t32.sh_offset;
				pmin = i;
				name = c->t32.sh_name;
			}
			c = c->suivant;
		}
		while (min > pcur)
		{
			write_quarter_word(0, dest.f);
			pcur++;
		}
		write_section(a, dest, pmin);

		// CHERCHER SECTION DANS 2EME FICHIER .O

		c2 = b.s.tete;
		c2_compt = 0;
		while (c2->t32.sh_name != name && c2->suivant != NULL)
		{
			c2 = c2->suivant;
			c2_compt++;
		}

		if (c2->t32.sh_name == name)
		{
			write_section(b, dest, c2_compt);
			tab2[c2_compt] = 1;
		}

		// FIN RECHERCHE

		compt++;
		tab[pmin] = 1;
		min = 100000;
		pmin = 0;
		c = a.s.tete;
	}



	// ECRITURE DES SECTIONS UNIQUES DANS B	
	c2 = b.s.tete;
	c = dest.s.tete;


	OneHeader *suiv = malloc (sizeof(OneHeader));
	suiv->suivant = NULL;

	while (c->suivant != NULL)
		c = c->suivant;
	
	for(int i=0 ; i<e_shnum2 ; i++)
	{
		if (tab2[i] == 0)
		{
			write_section(b, dest, i);
		}
		
		c2 = c2->suivant;
	}
	


	// ECRITURE SECTION HEADER
	write_section_header(dest);
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
		write_half_word(dest.h.h32.e_type, dest.f);
		write_half_word(dest.h.h32.e_machine, dest.f);
		
		write_word(dest.h.h32.e_version, dest.f);
		

		write_word(dest.h.h32.e_entry, dest.f);
		write_word(dest.h.h32.e_phoff, dest.f);
		write_word(dest.h.h32.e_shoff, dest.f);
		write_word(dest.h.h32.e_flags, dest.f);

		write_half_word(dest.h.h32.e_ehsize, dest.f);
		write_half_word(dest.h.h32.e_phentsize, dest.f);

		write_half_word(dest.h.h32.e_phnum, dest.f);
		write_half_word(dest.h.h32.e_shentsize, dest.f);

		write_half_word(dest.h.h32.e_shnum, dest.f);
		write_half_word(dest.h.h32.e_shstrndx, dest.f);
	}
	else if(dest.h.architecture==2)
	{
		write_half_word(dest.h.h64.e_type, dest.f);
		write_half_word(dest.h.h64.e_machine, dest.f);
		
		write_word(dest.h.h64.e_version, dest.f);
		

		write_double_word(dest.h.h64.e_entry, dest.f);
		write_double_word(dest.h.h64.e_phoff, dest.f);
		write_double_word(dest.h.h64.e_shoff, dest.f);
		write_word(dest.h.h64.e_flags, dest.f);

		write_half_word(dest.h.h64.e_ehsize, dest.f);
		write_half_word(dest.h.h64.e_phentsize, dest.f);

		write_half_word(dest.h.h64.e_phnum, dest.f);
		write_half_word(dest.h.h64.e_shentsize, dest.f);

		write_half_word(dest.h.h64.e_shnum, dest.f);
		write_half_word(dest.h.h64.e_shstrndx, dest.f);
	}
}

void write_section_header(OFile dest)
{
	int e_shnum = (dest.h.architecture == 1) ? dest.h.h32.e_shnum : dest.h.h64.e_shnum;

	OneHeader *c = dest.s.tete;

	for(int i=0 ; i<e_shnum ; i++)
	{
		write_word(c->t32.sh_name, dest.f);
		write_word(c->t32.sh_type, dest.f);
		write_word(c->t32.sh_flags, dest.f);;
		write_word(c->t32.sh_addr, dest.f);
		write_word(c->t32.sh_offset, dest.f);
		write_word(c->t32.sh_size, dest.f);
		write_word(c->t32.sh_link, dest.f);
		write_word(c->t32.sh_info, dest.f);
		write_word(c->t32.sh_addralign, dest.f);
		write_word(c->t32.sh_entsize, dest.f);

		c = c->suivant;
	}
}

void write_section(OFile source, OFile dest, int i)
{
	OneHeader *c = source.s.tete;
	char cc;

	for(int j=0 ; j<i ; j++)
		c = c->suivant;

	fseek(source.f, c->t32.sh_offset ,SEEK_SET);

	for(int j=0 ; j < c->t32.sh_size / 2; j++)
	{
		cc = read_unsigned_char(source.f);
		fwrite(&cc, 1, 1, dest.f);
	}
}




