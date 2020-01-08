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
		s.t[i].sh_name = read_word(f);
		s.t[i].sh_type = read_word(f);
		s.t[i].sh_flags = read_word(f);
		s.t[i].sh_addr = read_word(f);
		s.t[i].sh_offset = read_word(f);
		s.t[i].sh_size = read_word(f);
		s.t[i].sh_link = read_word(f);
		s.t[i].sh_info = read_word(f);
		s.t[i].sh_addralign = read_word(f);
		s.t[i].sh_entsize = read_word(f);
	}

	printf("Section Headers:\n");
	printf("[Nr]\tName\tType\tFlags\tAddr\tOffset\tSize\tLink\tInfo\tAddralign\tEntsize\n");

	for(int i=0 ; i<s.nb ; i++){
		printf("[%d]\t", i);
		printf("%x\t", s.t[i].sh_name);
		printf("%x\t", s.t[i].sh_type);
		printf("%x\t", s.t[i].sh_flags);
		printf("%x\t", s.t[i].sh_addr);
		printf("%x\t", s.t[i].sh_offset);
		printf("%x\t", s.t[i].sh_size);
		printf("%x\t", s.t[i].sh_link);
		printf("%x\t", s.t[i].sh_info);
		printf("%x\t\t", s.t[i].sh_addralign);
		printf("%x\n", s.t[i].sh_entsize);
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
	fseek(f, secHead.t[i].sh_offset ,SEEK_SET);

	for(int j=0 ; j < secHead.t[i].sh_size / 2; j++)
		printf("%08x ", read_quarter_word(f));
	printf("\n\n");
}

SymTab read_table_symboles(FILE * f, SecHead secHead, SymTab symTab)
{
	for (int i = 0; i < secHead.nb; i++) {
		//Check for type == SHT_REL or SHT_RELA
		if(secHead.t[i].sh_type == SHT_REL || secHead.t[i].sh_type == SHT_RELA){
			//Get the index of the symbol table section header
			int index = secHead.t[i].sh_link;

			symTab.nb = secHead.t[index].sh_size / 16;

			fseek(f, secHead.t[index].sh_offset,SEEK_SET);

			for(int i=0 ; i < symTab.nb ; i++){
				symTab.t[i].st_name = read_word(f);
				symTab.t[i].st_value = read_word(f);
				symTab.t[i].st_size = read_word(f);
				symTab.t[i].st_info = read_quarter_word(f);
				symTab.t[i].st_other = read_quarter_word(f);
				symTab.t[i].st_shndx = read_half_word(f);
			}
		}
	}

	return symTab;
}

void print_table_symboles(SymTab symTab){
	printf("La table de symboles contient %d entrées :\n", symTab.nb);
	printf("[Nr]\tName     Value    Size     Info     Other    shndx\n");
	for(int i=0 ; i < symTab.nb ; i++){
		printf("[%d]\t", i);
		printf("%08x ", symTab.t[i].st_name);
		printf("%08x ", symTab.t[i].st_value);
		printf("%08x ", symTab.t[i].st_size);
		printf("%08x ", symTab.t[i].st_info);
		printf("%08x ", symTab.t[i].st_other);
		printf("%08x\n", symTab.t[i].st_shndx);
	}
	printf("\n\n");
}

void read_table_reimplantation(FILE * f, SecHead secHead, SymTab symTab)
{
	for (int i = 0; i < secHead.nb; i++) {
		// Chek type = SHT_REL ou RELA
		if(secHead.t[i].sh_type == SHT_REL || secHead.t[i].sh_type == SHT_RELA){
			printf("Section de réadressage '.rel.text' à l'adresse de décalage 0x%x contient %d entrées :\n", secHead.t[i].sh_offset, secHead.t[i].sh_size / 8);
			printf("Offset   Info     Type     Val.-sym Noms-symboles\n");

			fseek(f, secHead.t[i].sh_offset ,SEEK_SET); //Offset
			for(int j=0 ; j < secHead.t[i].sh_size / 8 ; j++){ // Size
				printf("%08x ", read_word(f));
				int info = read_word(f);
				printf("%08x ", info);
				printf("%08x ", ELF32_R_TYPE(info)); // type

				printf("%08x ", symTab.t[ELF32_R_SYM(info)].st_value);
				printf("%08x\n", ELF32_R_INFO(ELF32_R_SYM(info),symTab.t[ELF32_R_SYM(info)].st_name));
			}
			printf("\n\n");
		}
	}
}

void print_string_table(Elf32_Ehdr head, SecHead secHead)
{
	int index=head.e_shstrndx & 0xff;
	printf("%x\t",secHead.t[index].sh_name);
	printf("%x\n",index);
}