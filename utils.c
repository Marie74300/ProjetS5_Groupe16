#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "utils.h"

int power(int a, int b)
{
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

int read_double_word (FILE *f)
{
	int c;
	fread(&c, 8, 1, f);
	return c;
}

Elf_Header read_header(FILE * f)
{
	Elf_Header head;
	int a;

	for (int i=0 ; i<16 ; i++)
	{
		a = read_quarter_word(f);
		head.h32.e_ident[i] = a;
		head.h64.e_ident[i] = a;
	}

	if (head.h32.e_ident[EI_CLASS] == 1)
	{
		head.architecture = 1;
		
		head.h32.e_type = read_half_word(f);
		head.h32.e_machine = read_half_word(f);
		head.h32.e_version = read_word(f);

		head.h32.e_entry = read_word(f);
		head.h32.e_phoff = read_word(f);
		head.h32.e_shoff = read_word(f);
		head.h32.e_flags = read_word(f);

		head.h32.e_ehsize = read_half_word(f);
		head.h32.e_phentsize = read_half_word(f);

		head.h32.e_phnum = read_half_word(f);
		head.h32.e_shentsize = read_half_word(f);

		head.h32.e_shnum = read_half_word(f);
		head.h32.e_shstrndx = read_half_word(f);
	}

	else if (head.h32.e_ident[EI_CLASS] == 2)
	{
		head.architecture = 2;
		
		head.h64.e_type = read_half_word(f);
		head.h64.e_machine = read_half_word(f);
		head.h64.e_version = read_word(f);

		head.h64.e_entry = read_double_word(f);
		head.h64.e_phoff = read_double_word(f);
		head.h64.e_shoff = read_double_word(f);

		head.h64.e_flags = read_word(f);

		head.h64.e_ehsize = read_half_word(f);
		head.h64.e_phentsize = read_half_word(f);

		head.h64.e_phnum = read_half_word(f);
		head.h64.e_shentsize = read_half_word(f);

		head.h64.e_shnum = read_half_word(f);
		head.h64.e_shstrndx = read_half_word(f);
	}

	return head;
}

void print_header(Elf_Header h)
{
	printf("\nELF Header:\n");
	
	if (h.architecture == 1){
		printf("  Magic: ");
		for(int i=0 ; i<16 ; i++)
			printf("%x ", h.h32.e_ident[i]);
		printf("\n");

		printf("  Class: ");
		printf("%hd\n", h.h32.e_ident[EI_CLASS]);

		printf("  Data: ");
		printf("%hd\n", h.h32.e_ident[EI_DATA]);

		printf("  Version: ");
		printf("%hd\n", h.h32.e_ident[EI_VERSION]);

		printf("  OS/ABI: ");
		printf("%hd\n", h.h32.e_ident[EI_OSABI]);

		printf("  ABI Version: ");
		printf("%hd\n", h.h32.e_ident[EI_ABIVERSION]);

		printf("  Type: ");
		printf("%hd\n", h.h32.e_type);

		printf("  Machine: ");
		printf("%hd\n", h.h32.e_machine);

		printf("  Version: ");
		printf("%hd\n", h.h32.e_version);

		printf("  Entry point address: ");
		printf("%hd\n", h.h32.e_entry);

		printf("  Start of program headers: ");
		printf("%hd\n", h.h32.e_phoff);

		printf("  Start of section headers: ");
		printf("%hd\n", h.h32.e_shoff);

		printf("  Flags: ");
		printf("%hd\n", h.h32.e_flags);

		printf("  Size of this header: ");
		printf("%hd\n", h.h32.e_ehsize);

		printf("  Size of program headers: ");
		printf("%hd\n", h.h32.e_phentsize);

		printf("  Number of program headers: ");
		printf("%hd\n", h.h32.e_phnum);

		printf("  Size of section headers: ");
		printf("%hd\n", h.h32.e_shentsize);

		printf("  Number of section headers: ");
		printf("%hd\n", h.h32.e_shnum);

		printf("  Section header string table index: ");
		printf("%hd\n\n", h.h32.e_shstrndx);
	}
	else {
		printf("  Magic: ");
		for(int i=0 ; i<16 ; i++)
			printf("%x ", h.h64.e_ident[i]);
		printf("\n");

		printf("  Class: ");
		printf("%hd\n", h.h64.e_ident[EI_CLASS]);

		printf("  Data: ");
		printf("%hd\n", h.h64.e_ident[EI_DATA]);

		printf("  Version: ");
		printf("%hd\n", h.h64.e_ident[EI_VERSION]);

		printf("  OS/ABI: ");
		printf("%hd\n", h.h64.e_ident[EI_OSABI]);

		printf("  ABI Version: ");
		printf("%hd\n", h.h64.e_ident[EI_ABIVERSION]);

		printf("  Type: ");
		printf("%hd\n", h.h64.e_type);

		printf("  Machine: ");
		printf("%hd\n", h.h64.e_machine);

		printf("  Version: ");
		printf("%hd\n", h.h64.e_version);

		printf("  Entry point address: ");
		printf("%ld\n", h.h64.e_entry);

		printf("  Start of program headers: ");
		printf("%ld\n", h.h64.e_phoff);

		printf("  Start of section headers: ");
		printf("%ld\n", h.h64.e_shoff);

		printf("  Flags: ");
		printf("%hd\n", h.h64.e_flags);

		printf("  Size of this header: ");
		printf("%hd\n", h.h64.e_ehsize);

		printf("  Size of program headers: ");
		printf("%hd\n", h.h64.e_phentsize);

		printf("  Number of program headers: ");
		printf("%hd\n", h.h64.e_phnum);

		printf("  Size of section headers: ");
		printf("%hd\n", h.h64.e_shentsize);

		printf("  Number of section headers: ");
		printf("%hd\n", h.h64.e_shnum);

		printf("  Section header string table index: ");
		printf("%hd\n\n", h.h64.e_shstrndx);
	}
}

SecHead read_section_headers(FILE * f, Elf_Header h)
{
	int e_shoff = (h.architecture == 1) ? h.h32.e_shoff : h.h64.e_shoff;
	int e_shnum = (h.architecture == 1) ? h.h32.e_shnum : h.h64.e_shnum;

	SecHead s;
	s.nb = e_shnum;
	s.tete = NULL;

	fseek(f, e_shoff ,SEEK_SET);

	OneHeader *c, *p;
	c = malloc (sizeof(OneHeader));
	s.tete = c;

	
	for(int i=0 ; i<e_shnum ; i++)
	{
		c->t.sh_name = read_word(f);
		c->t.sh_type = read_word(f);
		c->t.sh_flags = read_word(f);
		c->t.sh_addr = read_word(f);
		c->t.sh_offset = read_word(f);
		c->t.sh_size = read_word(f);
		c->t.sh_link = read_word(f);
		c->t.sh_info = read_word(f);
		c->t.sh_addralign = read_word(f);
		c->t.sh_entsize = read_word(f);

		p = c;
		c = malloc (sizeof(OneHeader));
		p->suivant = c;
	}

	return s;
}

void print_section_headers(SecHead s)
{
	printf("Section Headers:\n");
	printf("[Nr]\tName\tType\tFlags\tAddr\tOffset\tSize\tLink\tInfo\tAddralign\tEntsize\n");

	OneHeader *c = s.tete;

	for(int i=0 ; i<s.nb ; i++)
	{
		printf("[%d]\t", i);

		printf("%x\t", c->t.sh_name);
		printf("%x\t", c->t.sh_type);
		printf("%x\t", c->t.sh_flags);
		printf("%x\t", c->t.sh_addr);
		printf("%x\t", c->t.sh_offset);
		printf("%x\t", c->t.sh_size);
		printf("%x\t", c->t.sh_link);
		printf("%x\t", c->t.sh_info);
		printf("%x\t\t", c->t.sh_addralign);
		printf("%x\n", c->t.sh_entsize);

		c = c->suivant;
	}
	printf("\n");

}


void print_section(FILE * f, SecHead s, int i)
{
	printf("Affichage de la section %d:\n", i);

	OneHeader *c = s.tete;
	for(int j=0 ; j<i ; j++)
		c = c->suivant;

	fseek(f, c->t.sh_offset ,SEEK_SET);

	for(int j=0 ; j < c->t.sh_size / 2; j++)
		printf("%08x ", read_quarter_word(f));
	printf("\n\n");
}


SymTab read_table_symboles(FILE * f, SecHead s)
{
	SymTab st;
	st.tete = NULL;

	OneSymbol *current, *precedent;
	current = malloc (sizeof(OneSymbol));
	st.tete = current;

	OneHeader *c = s.tete;
	OneHeader *c2 = s.tete;

	for (int i = 0; i < s.nb; i++) 
	{
		//Check for type == SHT_REL or SHT_RELA
		if(c->t.sh_type == SHT_REL || c->t.sh_type == SHT_RELA)
		{
			//Get the index of the symbol table section header
			int index = c->t.sh_link;

			for(int j=0 ; j<index ; j++)
				c2 = c2->suivant;

			st.nb = c2->t.sh_size / 16;

			fseek(f, c2->t.sh_offset, SEEK_SET);

			for(int j=0 ; j < st.nb ; j++)
			{
				current->t.st_name = read_word(f);
				current->t.st_value = read_word(f);
				current->t.st_size = read_word(f);
				current->t.st_info = read_quarter_word(f);
				current->t.st_other = read_quarter_word(f);
				current->t.st_shndx = read_half_word(f);

				precedent = current;
				current = malloc (sizeof(OneSymbol));
				precedent->suivant = current;
			}

		}
		c = c->suivant;
		c2 = s.tete;
	}

	return st;
}

void print_table_symboles(SymTab st)
{
	printf("La table de symboles contient %d entrées :\n", st.nb);
	printf("[Nr]\tName     Value    Size     Info     Other    shndx\n");

	OneSymbol *current = st.tete;

	for(int i=0 ; i < st.nb ; i++)
	{
		printf("[%d]\t", i);
		printf("%08x ", current->t.st_name);
		printf("%08x ", current->t.st_value);
		printf("%08x ", current->t.st_size);
		printf("%08x ", current->t.st_info);
		printf("%08x ", current->t.st_other);
		printf("%08x\n", current->t.st_shndx);

		current = current->suivant;
	}
	printf("\n\n");
}



void read_table_reimplantation(FILE * f, SecHead s, SymTab st)
{
	OneHeader *c = s.tete;
	OneSymbol *sc = st.tete;

	for (int i = 0; i < s.nb; i++) 
	{
		// Chek type = SHT_REL ou RELA
		if(c->t.sh_type == SHT_REL || c->t.sh_type == SHT_RELA)
		{
			printf("Section de réadressage '.rel.text' à l'adresse de décalage 0x%x contient %d entrées :\n", c->t.sh_offset, c->t.sh_size / 8);
			printf("Offset   Info     Type     Val.-sym Noms-symboles\n");

			fseek(f, c->t.sh_offset ,SEEK_SET); //Offset

			for(int j=0 ; j < c->t.sh_size / 8 ; j++) // Size
			{ 
				printf("%08x ", read_word(f));
				int info = read_word(f);
				printf("%08x ", info);
				printf("%08x ", ELF32_R_TYPE(info)); // type

				for (int x=0 ; x< ELF32_R_SYM(info) ; x++)
					sc = sc->suivant;

				printf("%08x ", sc->t.st_value);
				printf("%08x\n", ELF32_R_INFO(ELF32_R_SYM(info), sc->t.st_name));

				sc = st.tete;
			}
			printf("\n\n");
		}
		c = c->suivant;
	}
}

void print_string_table(Elf_Header head, SecHead s)
{
	printf("Affichage de la table des string : \n");

	int e_shstrndx = (head.architecture == 1) ? head.h32.e_shstrndx : head.h64.e_shstrndx;
	int index = e_shstrndx & 0xff;

	OneHeader *c = s.tete;
	for(int i=0 ; i<index ; i++)
		c = c->suivant;

	printf("%x\t",c->t.sh_name);
	printf("%x\n",index);
}


