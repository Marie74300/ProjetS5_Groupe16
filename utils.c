#include <stdio.h>
#include <elf.h>
#include "utils.h"

int power(int a, int b){
	int r=1;
	for (int i=0 ; i<b ; i++)
		r *= a;
	return r;
}

int read_quarter_word (FILE *f)
{
	int c;
	fread(&c, 1, 1, f);
	c = c & 0x00FF;
	return c;
}

int read_half_word (FILE *f)
{
	int c;
	fread(&c, 2, 1, f);
	return c;
}

int read_word (FILE *f)
{
	int c;
	fread(&c, 4, 1, f);
	return c;
}

Elf32_Ehdr read_header(FILE * f)
{
	Elf32_Ehdr head;

	for (int i=0 ; i<16 ; i++)
		head.e_ident[i] = read_quarter_word(f);
	
	head.e_type = read_half_word(f);
	head.e_machine = read_half_word(f);
	head.e_version = read_word(f);

	head.e_entry = read_word(f);
	head.e_phoff = read_word(f);
	head.e_shoff = read_word(f);
	head.e_flags = read_word(f);

	head.e_ehsize = read_half_word(f);
	head.e_phentsize = read_half_word(f);

	head.e_phnum = read_half_word(f);
	head.e_shentsize = read_half_word(f);

	head.e_shnum = read_half_word(f);
	head.e_shstrndx = read_half_word(f);

	return head;
}

void print_header(Elf32_Ehdr h)
{
	printf("\nELF Header:\n");

	printf("  Magic: ");
	for(int i=0 ; i<16 ; i++)
		printf("%x ", h.e_ident[i]);
	printf("\n");

	printf("  Class: ");
	printf("%hd\n", h.e_ident[EI_CLASS]);

	printf("  Data: ");
	printf("%hd\n", h.e_ident[EI_DATA]);

	printf("  Version: ");
	printf("%hd\n", h.e_ident[EI_VERSION]);

	printf("  OS/ABI: ");
	printf("%hd\n", h.e_ident[EI_OSABI]);

	printf("  ABI Version: ");
	printf("%hd\n", h.e_ident[EI_ABIVERSION]);

	printf("  Type: ");
	printf("%hd\n", h.e_type);

	printf("  Machine: ");
	printf("%hd\n", h.e_machine);

	printf("  Version: ");
	printf("%hd\n", h.e_version);

	printf("  Entry point address: ");
	printf("%hd\n", h.e_entry);

	printf("  Start of program headers: ");
	printf("%hd\n", h.e_phoff);

	printf("  Start of section headers: ");
	printf("%hd\n", h.e_shoff);

	printf("  Flags: ");
	printf("%hd\n", h.e_flags);

	printf("  Size of this header: ");
	printf("%hd\n", h.e_ehsize);

	printf("  Size of program headers: ");
	printf("%hd\n", h.e_phentsize);

	printf("  Number of program headers: ");
	printf("%hd\n", h.e_phnum);

	printf("  Size of section headers: ");
	printf("%hd\n", h.e_shentsize);

	printf("  Number of section headers: ");
	printf("%hd\n", h.e_shnum);

	printf("  Section header string table index: ");
	printf("%hd\n\n", h.e_shstrndx);
}

void read_section_headers(FILE * f, Elf32_Ehdr h, SecHead s)
{
	fseek(f, h.e_shoff ,SEEK_SET);

	for(int i=0 ; i<h.e_shnum ; i++){
		s.t[i][0] = read_word(f);
		s.t[i][1] = read_word(f);
		s.t[i][2] = read_word(f);
		s.t[i][3] = read_word(f);
		s.t[i][4] = read_word(f);
		s.t[i][5] = read_word(f);
		s.t[i][6] = read_word(f);
		s.t[i][7] = read_word(f);
		s.t[i][8] = read_word(f);
		s.t[i][9] = read_word(f);
	}

	printf("Section Headers:\n");
	printf("[Nr]\tName\tType\tFlags\tAddr\tOffset\tSize\tLink\tInfo\tAddralign\tEntsize\n");

	for(int i=0 ; i<s.nb ; i++){
		printf("[%d]\t", i);
		printf("%x\t", s.t[i][0]);
		printf("%x\t", s.t[i][1]);
		printf("%x\t", s.t[i][2]);
		printf("%x\t", s.t[i][3]);
		printf("%x\t", s.t[i][4]);
		printf("%x\t", s.t[i][5]);
		printf("%x\t", s.t[i][6]);
		printf("%x\t", s.t[i][7]);
		printf("%x\t\t", s.t[i][8]);
		printf("%x\n", s.t[i][9]);
	}
	printf("\n");
}

/*void print_section_headers(SecHead s)
{
	printf("Section Headers:\n");
	printf("[Nr]\tName\tType\tFlags\tAddr\tOffset\tSize\tLink\tInfo\tAddralign\tEntsize\n");
	
	for(int i=0 ; i<s.nb ; i++){
		printf("[%d]\t", i);
		printf("%x\t", s.t[i][0]);
		printf("%x\t", s.t[i][1]);
		printf("%x\t", s.t[i][2]);
		printf("%x\t", s.t[i][3]);
		printf("%x\t", s.t[i][4]);
		printf("%x\t", s.t[i][5]);
		printf("%x\t", s.t[i][6]);
		printf("%x\t", s.t[i][7]);
		printf("%x\t\t", s.t[i][8]);
		printf("%x\n", s.t[i][9]);
	}
	printf("\n");
}*/

void print_section(FILE * f, int i)
{
	printf("Affichage de la section 1:\n");
	fseek(f, 52 ,SEEK_SET);

	for(int j=0 ; j<32 ; j++)
		printf("%x ", read_quarter_word(f));
	printf("\n\n");
}

void read_table_symboles(FILE * f, Elf32_Ehdr h)
{
	printf("Affichage de la table des symboles:\n");
	printf("[Nr]\tValue\tSize\tInfo\tOther\tshndx\n");
	fseek(f, 2*16*16+2*16+0 ,SEEK_SET);

	for(int j=0 ; j<15 ; j++){
		printf("[%d]\t", j);
		printf("%x\t", read_word(f));
		printf("%x\t", read_word(f));
		printf("%x\t", read_word(f));
		printf("%x\t", read_quarter_word(f));
		printf("%x\t", read_quarter_word(f));
		printf("%x\n", read_half_word(f));
	}
	printf("\n\n");
}


