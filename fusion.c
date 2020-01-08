#include "fusion.h"
void write_quarter_word (int c, FILE *f)
{
	fwrite(c, 1, 1, f);
}

void write_half_word (int c, FILE *f)
{
	fwrite(c, 2, 1, f);
}

void write_word (int c, FILE *f)
{
	fwrite(c, 4, 1, f);
}

void write_double_word (int c, FILE *f)
{
	fwrite(c, 8, 1, f);
}


void fusion(OFile a, OFile b, OFile dest)
{
	dest.h=a.h;
	dest.s=a.s;
	// Les tables de strings sont des sections
//	dest.string1=a.string1;
//	dest.string2=a.string2;
	dest.st=a.st;
	dest.r=a.r;
	
	// FUSION & RENUMEROTATION SECTIONS





	write_header(dest);
	write_section(dest);
	write_section_header(dest);
}

void write_header(OFile dest){
	if(dest.h.architecture==1){
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
	else if(dest.h.architecture==2){
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

void write_section(FILE * lec, FILE * ec, SecHead s, int i)
{
	OneHeader *c = s.tete;
	char cc;

	for(int j=0 ; j<i ; j++)
		c = c->suivant;

	fseek(lec, c->t.sh_offset ,SEEK_SET);

	for(int j=0 ; j < c->t.sh_size / 2; j++)
	{
		cc = read_quarter_word(lec);
		fwrite(&cc, 1, 1, ec);
	}
}

void write_section_header(dest){

}