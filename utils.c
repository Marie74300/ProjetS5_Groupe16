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

SecHead read_section_headers(FILE * f, Elf32_Ehdr h, SecHead s)
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

	return s;
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

void print_section(FILE * f, SecHead secHead, int i)
{
	printf("Affichage de la section %d:\n", i);
	fseek(f, secHead.t[i][4] ,SEEK_SET);

	for(int j=0 ; j < secHead.t[i][5] / 2; j++)
		printf("%x ", read_quarter_word(f));
	printf("\n\n");
}

SymTab read_table_symboles(FILE * f, SecHead secHead, SymTab symTab)
{
	for (int i = 0; i < secHead.nb; i++) {
		//Check for type == SHT_REL or SHT_RELA
		if(secHead.t[i][1] == SHT_REL || secHead.t[i][1] == SHT_RELA){
			//Get the index of the symbol table section header
			int index = secHead.t[i][6];

			symTab.nb = secHead.t[index][5] / 16;

			fseek(f, secHead.t[index][4],SEEK_SET);

			for(int i=0 ; i < symTab.nb ; i++){
				symTab.t[i][0] = read_word(f);
				symTab.t[i][1] = read_word(f);
				symTab.t[i][2] = read_word(f);
				symTab.t[i][3] = read_quarter_word(f);
				symTab.t[i][4] = read_quarter_word(f);
				symTab.t[i][5] = read_half_word(f);
			}
		}
	}

	return symTab;
}

void print_table_symboles(SymTab symTab){
	printf("La table de symboles contient %d entrées :\n", symTab.nb);
	printf("[Nr]\tName\tValue\tSize\tInfo\tOther\tshndx\n");
	for(int i=0 ; i < symTab.nb ; i++){
		printf("[%d]\t", i);
		printf("%x\t", symTab.t[i][0]);
		printf("%x\t", symTab.t[i][1]);
		printf("%x\t", symTab.t[i][2]);
		printf("%x\t", symTab.t[i][3]);
		printf("%x\t", symTab.t[i][4]);
		printf("%x\n", symTab.t[i][5]);
	}
	printf("\n\n");
}

void read_table_reimplantation(FILE * f, SecHead secHead, SymTab symTab)
{
	for (int i = 0; i < secHead.nb; i++) {
		// Chek type = SHT_REL ou RELA
		if(secHead.t[i][1] == SHT_REL || secHead.t[i][1] == SHT_RELA){
			printf("Section de réadressage '.rel.text' à l'adresse de décalage 0x%x contient %d entrées :\n", secHead.t[i][4], secHead.t[i][5] / 8);
			printf("Décal\tInfo\tType\tVal.-sym\tNoms-symboles\n");

			fseek(f, secHead.t[i][4] ,SEEK_SET); //Offset
			for(int j=0 ; j < secHead.t[i][5] / 8 ; j++){ // Size
				printf("%x\t", read_word(f));
				int info = read_word(f);
				printf("%x\t", info);
				printf("%x\t", info & 0xFF); // type

				printf("%x\t\t", symTab.t[info >> 8][1]); // symbol value
				printf("%x\n", symTab.t[info >> 8][0]); // symbol name
			}
			printf("\n\n");
		}
	}
}
