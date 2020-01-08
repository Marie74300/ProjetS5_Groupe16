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
		printf("  Magic:   ");
		for(int i=0 ; i<16 ; i++)
			printf("%02x ", h.h32.e_ident[i]);
		printf("\n");

		printf("  Class:                             ");
		printf("%hd ", h.h32.e_ident[EI_DATA]);
		if(h.h32.e_ident[EI_CLASS]==0){printf("ELFCLASSNONE\n");}else if(h.h32.e_ident[EI_CLASS]==1){printf("ELFCLASS32\n");}else if(h.h32.e_ident[EI_CLASS]==2){printf("ELFCLASS64\n");}else{printf("\n");}

		printf("  Data:                              ");
		printf("%hd ", h.h32.e_ident[EI_DATA]);
		if(h.h32.e_ident[EI_DATA]==0){printf("ELFDATANONE\n");}else if(h.h32.e_ident[EI_DATA]==1){printf("ELFDATA2LSB\n");}else if(h.h32.e_ident[EI_DATA]==2){printf("ELFDATA2MSB\n");}else{printf("\n");}

		printf("  Version:                           ");
		printf("%hd ", h.h32.e_ident[EI_VERSION]);
		if(h.h32.e_ident[EI_VERSION]==0){printf("EV_NONE\n");}else if(h.h32.e_ident[EI_VERSION]==1){printf("EV_CURRENT\n");}else{printf("\n");}

		printf("  OS/ABI:                            ");
		printf("%hd\n", h.h32.e_ident[EI_OSABI]);

		printf("  ABI Version:                       ");
		printf("%hd\n", h.h32.e_ident[EI_ABIVERSION]);

		printf("  Type:                              ");
		printf("%hd ", h.h32.e_type);
		if(h.h32.e_type==0){printf("ET_NONE\n");}else if(h.h32.e_type==1){printf("ET_REL\n");}else if(h.h32.e_type==2){printf("ET_EXEC\n");}else if(h.h32.e_type==3){printf("ET_DYN\n");}else if(h.h32.e_type==4){printf("ET_CORE\n");}else if(h.h32.e_type==0xff00){printf("ET_LOPROC\n");}else if(h.h32.e_type==0xffff){printf("ET_HIPROC\n");}else{printf("\n");}

		printf("  Machine:                           ");
		printf("%hd ", h.h32.e_machine);
		if(h.h32.e_machine==40){printf("ARM\n");}else{printf("\n");}

		printf("  Version:                           ");
		printf("0x%x\n", h.h32.e_version);

		printf("  Entry point address:               ");
		printf("0x%x\n", h.h32.e_entry);

		printf("  Start of program headers:          ");
		printf("%hd (bytes into file)\n", h.h32.e_phoff);

		printf("  Start of section headers:          ");
		printf("%hd (bytes into file)\n", h.h32.e_shoff);

		printf("  Flags:                             ");
		printf("%hd\n", h.h32.e_flags);

		printf("  Size of this header:               ");
		printf("%hd (bytes)\n", h.h32.e_ehsize);

		printf("  Size of program headers:           ");
		printf("%hd (bytes)\n", h.h32.e_phentsize);

		printf("  Number of program headers:         ");
		printf("%hd\n", h.h32.e_phnum);

		printf("  Size of section headers:           ");
		printf("%hd (bytes)\n", h.h32.e_shentsize);

		printf("  Number of section headers:         ");
		printf("%hd\n", h.h32.e_shnum);

		printf("  Section header string table index: ");
		printf("%hd\n\n", h.h32.e_shstrndx);
	}
	else {
		printf("  Magic:   ");
		for(int i=0 ; i<16 ; i++)
			printf("%02x ", h.h64.e_ident[i]);
		printf("\n");

				printf("  Class:                             ");
		printf("%hd ", h.h64.e_ident[EI_DATA]);
		if(h.h64.e_ident[EI_CLASS]==0){printf("ELFCLASSNONE\n");}else if(h.h64.e_ident[EI_CLASS]==1){printf("ELFCLASS32\n");}else if(h.h64.e_ident[EI_CLASS]==2){printf("ELFCLASS64\n");}else{printf("\n");}

		printf("  Data:                              ");
		printf("%hd ", h.h64.e_ident[EI_DATA]);
		if(h.h64.e_ident[EI_DATA]==0){printf("ELFDATANONE\n");}else if(h.h64.e_ident[EI_DATA]==1){printf("ELFDATA2LSB\n");}else if(h.h64.e_ident[EI_DATA]==2){printf("ELFDATA2MSB\n");}else{printf("\n");}

		printf("  Version:                           ");
		printf("%hd ", h.h32.e_ident[EI_VERSION]);
		if(h.h64.e_ident[EI_VERSION]==0){printf("EV_NONE\n");}else if(h.h64.e_ident[EI_VERSION]==1){printf("EV_CURRENT\n");}else{printf("\n");}

		printf("  OS/ABI:                            ");
		printf("%hd\n", h.h64.e_ident[EI_OSABI]);

		printf("  ABI Version:                       ");
		printf("%hd\n", h.h64.e_ident[EI_ABIVERSION]);

		printf("  Type:                              ");
		printf("%hd ", h.h64.e_type);
		if(h.h64.e_type==0){printf("ET_NONE\n");}else if(h.h64.e_type==1){printf("ET_REL\n");}else if(h.h64.e_type==2){printf("ET_EXEC\n");}else if(h.h64.e_type==3){printf("ET_DYN\n");}else if(h.h64.e_type==4){printf("ET_CORE\n");}else if(h.h64.e_type==0xff00){printf("ET_LOPROC\n");}else if(h.h64.e_type==0xffff){printf("ET_HIPROC\n");}else{printf("\n");}

		printf("  Machine:                           ");
		printf("%hd ", h.h64.e_machine);
		if(h.h64.e_machine==40){printf("ARM\n");}else{printf("\n");}

		printf("  Version:                           ");
		printf("0x%x\n", h.h64.e_version);

		printf("  Entry point address:               ");
		printf("0x%lx\n", h.h64.e_entry);

		printf("  Start of program headers:          ");
		printf("%ld (bytes into file)\n", h.h64.e_phoff);

		printf("  Start of section headers:          ");
		printf("%ld (bytes into file)\n", h.h64.e_shoff);

		printf("  Flags:                             ");
		printf("%hd\n", h.h64.e_flags);

		printf("  Size of this header:               ");
		printf("%hd (bytes)\n", h.h64.e_ehsize);

		printf("  Size of program headers:           ");
		printf("%hd (bytes)\n", h.h64.e_phentsize);

		printf("  Number of program headers:         ");
		printf("%hd\n", h.h64.e_phnum);

		printf("  Size of section headers:           ");
		printf("%hd (bytes)\n", h.h64.e_shentsize);

		printf("  Number of section headers:         ");
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

void print_section_headers(SecHead s, StringTab string)
{
	printf("Section Headers:\n");
	printf("[Nr]\tName\t\tType\t\tAddr\tOff\tSize\tES\tFlg\tLk\tInf\tAl\n");

	OneHeader *c = s.tete;

	for(int i=0 ; i<s.nb ; i++)
	{
		printf("[%d]\t", i);

		//printf("%x\t", c->t.sh_name);
		//Ici l'affichage est adapté uniquement pour nos 2 exemples (es.o accessmem.o)
		if(i==2 || i==5 || i==6){printf("%s\t", string_pos(string, c->t.sh_name));}else {printf("%s\t\t", string_pos(string, c->t.sh_name));}
		if(c->t.sh_type==0){printf("SHT_NULL\t");}else if(c->t.sh_type==1){printf("SHT_PROGBITS\t");}else if(c->t.sh_type==2){printf("SHT_SYMTAB\t");}else if(c->t.sh_type==3){printf("SHT_STRTAB\t");}else if(c->t.sh_type==4){printf("SHT_RELA\t\t");}else if(c->t.sh_type==5){printf("SHT_HASH\t");}else if(c->t.sh_type==6){printf("SHT_DYNAMIC\t");}else if(c->t.sh_type==7){printf("SHT_NOTE\t");}else if(c->t.sh_type==8){printf("SHT_NOBITS\t");}else if(c->t.sh_type==9){printf("SHT_REL\t\t");}else if(c->t.sh_type==10){printf("SHT_SHLIB\t");}else if(c->t.sh_type==11){printf("SHT_DYNSYM\t");}else if(c->t.sh_type==0x70000000){printf("SHT_LOPROC\t");}else if(c->t.sh_type==0x7fffffff){printf("SHT_HIPROC\t");}else if(c->t.sh_type==0x80000000){printf("SHT_LOUSER\t");}else if(c->t.sh_type==0xffffffff){printf("SHT_HIUSER\t");}else{if(i==5){printf("%x\t", c->t.sh_type);}else{printf("%x\t\t", c->t.sh_type);}}
		printf("%x\t", c->t.sh_addr);
		printf("%x\t", c->t.sh_offset);
		printf("%x\t", c->t.sh_size);
		printf("%x\t", c->t.sh_entsize);
		printf("%x\t", c->t.sh_flags);
		printf("%x\t", c->t.sh_link);
		printf("%x\t", c->t.sh_info);
		printf("%x\n", c->t.sh_addralign);

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
		printf("%02x ", read_quarter_word(f));
	printf("\n\n");
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
		printf("%02x       ", current->t.st_info);
		printf("%02x       ", current->t.st_other);
		printf("%04x\n", current->t.st_shndx);

		current = current->suivant;
	}
	printf("\n\n");
}



ReimpTab read_table_reimplantation(FILE * f, SecHead s, SymTab st)
{
	OneHeader *c = s.tete;
	OneSymbol *sc = st.tete;

	ReimpTab r;
	r.tete = NULL;

	OneReimp *current, *precedent;
	current = malloc (sizeof(OneReimp));
	r.tete = current;


	for (int i = 0; i < s.nb; i++) 
	{
		// Chek type = SHT_REL ou RELA
		if(c->t.sh_type == SHT_REL || c->t.sh_type == SHT_RELA)
		{
			r.nb = c->t.sh_size / 8;
			r.offset = c->t.sh_offset;

			fseek(f, c->t.sh_offset ,SEEK_SET); //Offset

			for(int j=0 ; j < c->t.sh_size / 8 ; j++) // Size
			{ 
				/*printf("%08x ", read_word(f));
				int info = read_word(f);
				printf("%08x ", info);
				printf("%08x ", ELF32_R_TYPE(info)); // type

				for (int x=0 ; x< ELF32_R_SYM(info) ; x++)
					sc = sc->suivant;

				printf("%08x ", sc->t.st_value);
				printf("%08x\n", ELF32_R_INFO(ELF32_R_SYM(info), sc->t.st_name));*/

				current->t[0] = read_word(f);
				int info = read_word(f);
				current->t[1] = info;
				current->t[2] = ELF32_R_TYPE(info);

				for (int x=0 ; x< ELF32_R_SYM(info) ; x++)
					sc = sc->suivant;

				current->t[3] = sc->t.st_value;
				current->t[4] = ELF32_R_INFO(ELF32_R_SYM(info), sc->t.st_name);

				sc = st.tete;
				precedent = current;
				current = malloc (sizeof(OneReimp));
				precedent->suivant = current;
			}
			printf("\n\n");
		}
		c = c->suivant;
	}

	return r;
}

void print_table_reimp(ReimpTab r)
{
	printf("Section de réadressage '.rel.text' à l'adresse de décalage 0x%x contient %d entrées :\n", r.offset, r.nb);
	printf("[Nb]\tOffset   Info     Type     Val.-sym Noms-symboles\n");

	OneReimp *c = r.tete;

	for(int i=0 ; i < r.nb ; i++)
	{
		printf("[%d]\t", i);
		printf("%08x ", c->t[0]);
		printf("%08x ", c->t[1]);
		printf("%08x ", c->t[2]);
		printf("%08x ", c->t[3]);
		printf("%08x\n", c->t[4]);

		c = c->suivant;
	}
	printf("\n\n");
}

StringTab read_string_table(FILE * f, Elf_Header head, SecHead s, int nb)
{
	OneHeader *c = s.tete;
	for (int i=0 ; i<nb ; i++)
	{
		if (c->t.sh_type != SHT_STRTAB)
		{
			while(c->t.sh_type != SHT_STRTAB)
				c = c->suivant;
		}
		else
		{
			c = c->suivant->suivant;
		}
	}


	fseek(f, c->t.sh_offset ,SEEK_SET);

	StringTab string;
	string.nb = 0;

	OneString *current;
	OneString *precedent;

	char cc;
	int lg = 0;
	int compt = 0;

	current = malloc (sizeof(OneString));
	current->pos = 1;
	string.tete = current;

	for(int i=0 ; i<c->t.sh_size ; i++)
	{
		cc = read_quarter_word(f);
		

		if (cc != '\0')
		{
			current->t[lg] = cc;
			lg ++;
		}
		else if (compt != 0)
		{
			current->t[lg] = '\0';
	
			lg = 0;
			string.nb ++;

			precedent = current;
			current = malloc (sizeof(OneString));
			precedent->suivant = current;

			current->pos = compt + 1;
		}
		compt++;
	}
	return string;
}

void print_string_tab(StringTab string)
{
	printf("La table des chaines de caracteres contient %d entrées :\n", string.nb);

	OneString *current = string.tete;

	for(int i=0 ; i < string.nb ; i++)
	{
		printf("[%d] : ", i);
		printf("%s - ", current->t);
		printf("%x\n", current->pos);

		current = current->suivant;
	}
	printf("\n\n");
}

char * string_pos(StringTab string, int pos)
{
	OneString *current = string.tete;
	
	while (current != NULL && pos != current->pos)
		current = current->suivant;

	if(current == NULL) return "";
	return current->t;
}



void end(OFile a)
{
	OneHeader *c1 = a.s.tete;
	OneHeader *p1;
	while(c1 != NULL)
	{
		p1 = c1;
		c1 = c1->suivant;	
		free(p1);
	}
	
	OneSymbol *c2 = a.st.tete;
	OneSymbol *p2;
	while(c2 != NULL)
	{
		p2 = c2;
		c2 = c2->suivant;	
		free(p2);
	}

	OneReimp *c3 = a.r.tete;
	OneReimp *p3;
	while(c3 != NULL)
	{
		p3 = c3;
		c3 = c3->suivant;	
		free(p3);
	}

	OneString *c4 = a.string1.tete;
	OneString *p4;
	while(c4 != NULL)
	{
		p4 = c4;
		c4 = c4->suivant;	
		free(p4);
	}

	OneString *c5 = a.string2.tete;
	OneString *p5;
	while(c5 != NULL)
	{
		p5 = c5;
		c5 = c5->suivant;	
		free(p5);
	}
}

