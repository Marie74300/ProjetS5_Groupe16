#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "utils.h"
#include "es.h"

int power(int a, int b)
{
	int r=1;
	for (int i=0 ; i<b ; i++)
		r *= a;
	return r;
}



// HEADER

Elf_Header read_header(FILE * f)
{
	Elf_Header head;
	int a;

	for (int i=0 ; i<EI_NIDENT ; i++)
	{
		a = read_unsigned_char(f);
		head.h32.e_ident[i] = a;
		head.h64.e_ident[i] = a;
	}

	int endianess = head.h32.e_ident[EI_DATA];
	head.endianess = endianess;

	if (head.h32.e_ident[EI_CLASS] == 1)
	{
		head.architecture = 1;
		
		head.h32.e_type = read_Elf32_Half(f, endianess);
		head.h32.e_machine = read_Elf32_Half(f, endianess);
		head.h32.e_version = read_Elf32_Word(f, endianess);

		head.h32.e_entry = read_Elf32_Addr(f, endianess);
		head.h32.e_phoff = read_Elf32_Off(f, endianess);
		head.h32.e_shoff = read_Elf32_Off(f, endianess);
		head.h32.e_flags = read_Elf32_Word(f, endianess);

		head.h32.e_ehsize = read_Elf32_Half(f, endianess);
		head.h32.e_phentsize = read_Elf32_Half(f, endianess);

		head.h32.e_phnum = read_Elf32_Half(f, endianess);
		head.h32.e_shentsize = read_Elf32_Half(f, endianess);

		head.h32.e_shnum = read_Elf32_Half(f, endianess);
		head.h32.e_shstrndx = read_Elf32_Half(f, endianess);
	}

	else if (head.h64.e_ident[EI_CLASS] == 2)
	{
		head.architecture = 2;
		
		head.h64.e_type = read_Elf32_Half(f, endianess);
		head.h64.e_machine = read_Elf32_Half(f, endianess);
		head.h64.e_version = read_Elf32_Word(f, endianess);

		head.h64.e_entry = read_Elf64_Addr(f, endianess);
		head.h64.e_phoff = read_Elf64_Off(f, endianess);
		head.h64.e_shoff = read_Elf64_Off(f, endianess);

		head.h64.e_flags = read_Elf32_Off(f, endianess);

		head.h64.e_ehsize = read_Elf32_Half(f, endianess);
		head.h64.e_phentsize = read_Elf32_Half(f, endianess);

		head.h64.e_phnum = read_Elf32_Half(f, endianess);
		head.h64.e_shentsize = read_Elf32_Half(f, endianess);

		head.h64.e_shnum = read_Elf32_Half(f, endianess);
		head.h64.e_shstrndx = read_Elf32_Half(f, endianess);
	}

	return head;
}

void print_header(Elf_Header h)
{
	printf("\nELF Header:\n");
	
	if (h.architecture == 1){
		printf("  Magic:   ");
		for(int i=0 ; i<EI_NIDENT ; i++)
			printf("%02x ", h.h32.e_ident[i]);
		printf("\n");

		printf("  Class:                             ");
		//printf("%hd ", h.h32.e_ident[EI_CLASS]);
		if(h.h32.e_ident[EI_CLASS]==0){printf("ELFCLASSNONE\n");}else if(h.h32.e_ident[EI_CLASS]==1){printf("ELFCLASS32\n");}else if(h.h32.e_ident[EI_CLASS]==2){printf("ELFCLASS64\n");}else{printf("%hd\n", h.h32.e_ident[EI_CLASS]);}

		printf("  Data:                              ");
		//printf("%hd ", h.h32.e_ident[EI_DATA]);
		if(h.h32.e_ident[EI_DATA]==0){printf("ELFDATANONE\n");}else if(h.h32.e_ident[EI_DATA]==1){printf("ELFDATA2LSB\n");}else if(h.h32.e_ident[EI_DATA]==2){printf("ELFDATA2MSB\n");}else{printf("%hd\n", h.h32.e_ident[EI_DATA]);}

		printf("  Version:                           ");
		//printf("%hd ", h.h32.e_ident[EI_VERSION]);
		if(h.h32.e_ident[EI_VERSION]==0){printf("EV_NONE\n");}else if(h.h32.e_ident[EI_VERSION]==1){printf("EV_CURRENT\n");}else{printf("%hd\n", h.h32.e_ident[EI_VERSION]);}

		printf("  OS/ABI:                            ");
		//printf("%hd\n", h.h32.e_ident[EI_OSABI]);
		if(h.h32.e_ident[EI_OSABI]==0x00){printf("UNIX - Systeme V\n");}else if(h.h32.e_ident[EI_OSABI]==0x03){printf("Linux\n");}else if(h.h32.e_ident[EI_OSABI]==0x06){printf("Solaris\n");}else if(h.h32.e_ident[EI_OSABI]==0x0A){printf("UNIX - True64\n");}else{printf("%hd\n", h.h32.e_ident[EI_OSABI]);}

		printf("  ABI Version:                       ");
		printf("%hd\n", h.h32.e_ident[EI_ABIVERSION]);

		printf("  Type:                              ");
		//printf("%hd ", h.h32.e_type);
		if(h.h32.e_type==0){printf("ET_NONE\n");}else if(h.h32.e_type==1){printf("ET_REL\n");}else if(h.h32.e_type==2){printf("ET_EXEC\n");}else if(h.h32.e_type==3){printf("ET_DYN\n");}else if(h.h32.e_type==4){printf("ET_CORE\n");}else if(h.h32.e_type==0xff00){printf("ET_LOPROC\n");}else if(h.h32.e_type==0xffff){printf("ET_HIPROC\n");}else{printf("\n");}

		printf("  Machine:                           ");
		//printf("%hd ", h.h32.e_machine);
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
		for(int i=0 ; i<EI_NIDENT ; i++)
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



// SECTION HEADER

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

	if (h.architecture == 1){
		for(int i=0 ; i<e_shnum ; i++)
		{
			c->t32.sh_name = read_Elf32_Word(f, h.endianess);
			c->t32.sh_type = read_Elf32_Word(f, h.endianess);
			c->t32.sh_flags = read_Elf32_Word(f, h.endianess);
			c->t32.sh_addr = read_Elf32_Addr(f, h.endianess);
			c->t32.sh_offset = read_Elf32_Off(f, h.endianess);
			c->t32.sh_size = read_Elf32_Word(f, h.endianess);
			c->t32.sh_link = read_Elf32_Word(f, h.endianess);
			c->t32.sh_info = read_Elf32_Word(f, h.endianess);
			c->t32.sh_addralign = read_Elf32_Word(f, h.endianess);
			c->t32.sh_entsize = read_Elf32_Word(f, h.endianess);

			p = c;
			c = malloc (sizeof(OneHeader));
			p->suivant = c;
		}
	} else if (h.architecture==2) {
		for(int i=0 ; i<e_shnum ; i++)
		{
			c->t64.sh_name = read_Elf32_Word(f, h.endianess);
			c->t64.sh_type = read_Elf32_Word(f, h.endianess);
			c->t64.sh_flags = read_Elf64_Word(f, h.endianess);
			c->t64.sh_addr = read_Elf64_Addr(f, h.endianess);
			c->t64.sh_offset = read_Elf64_Off(f, h.endianess);
			c->t64.sh_size = read_Elf64_Word(f, h.endianess);
			c->t64.sh_link = read_Elf32_Word(f, h.endianess);
			c->t64.sh_info = read_Elf32_Word(f, h.endianess);
			c->t64.sh_addralign = read_Elf64_Word(f, h.endianess);
			c->t64.sh_entsize = read_Elf64_Word(f, h.endianess);

			p = c;
			c = malloc (sizeof(OneHeader));
			p->suivant = c;
		}
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
		print_string(string, c->t32.sh_name);
		if(i!=2 && i!=5 && i!=6){printf("\t");}
		if(c->t32.sh_type==0){printf("SHT_NULL\t");}else if(c->t32.sh_type==1){printf("SHT_PROGBITS\t");}else if(c->t32.sh_type==2){printf("SHT_SYMTAB\t");}else if(c->t32.sh_type==3){printf("SHT_STRTAB\t");}else if(c->t32.sh_type==4){printf("SHT_RELA\t\t");}else if(c->t32.sh_type==5){printf("SHT_HASH\t");}else if(c->t32.sh_type==6){printf("SHT_DYNAMIC\t");}else if(c->t32.sh_type==7){printf("SHT_NOTE\t");}else if(c->t32.sh_type==8){printf("SHT_NOBITS\t");}else if(c->t32.sh_type==9){printf("SHT_REL\t\t");}else if(c->t32.sh_type==10){printf("SHT_SHLIB\t");}else if(c->t32.sh_type==11){printf("SHT_DYNSYM\t");}else if(c->t32.sh_type==0x70000000){printf("SHT_LOPROC\t");}else if(c->t32.sh_type==0x7fffffff){printf("SHT_HIPROC\t");}else if(c->t32.sh_type==0x80000000){printf("SHT_LOUSER\t");}else if(c->t32.sh_type==0xffffffff){printf("SHT_HIUSER\t");}else if(c->t32.sh_type==0x70000003){printf("ARM_ATTRIBUTES\t");}else{if(i==5){printf("%x\t", c->t32.sh_type);}else{printf("%x\t\t", c->t32.sh_type);}}
		printf("%x\t", c->t32.sh_addr);
		printf("%x\t", c->t32.sh_offset);
		printf("%x\t", c->t32.sh_size);
		printf("%x\t", c->t32.sh_entsize);
		printf("%x\t", c->t32.sh_flags);
		printf("%x \t", c->t32.sh_link);
		printf("%x\t", c->t32.sh_info);
		printf("%x\n", c->t32.sh_addralign);

		c = c->suivant;
	}
	printf("\n");

}



// SECTION

void print_section(FILE * f, SecHead s, int i)
{
	printf("Affichage de la section %d:\n", i);

	OneHeader *c = s.tete;
	for(int j=0 ; j<i ; j++)
		c = c->suivant;

	fseek(f, c->t32.sh_offset ,SEEK_SET);

	for(int j=0 ; j < c->t32.sh_size ; j++)
	{
		printf("%02x ", read_unsigned_char(f));
		if ((j+1)%4 == 0) printf("\t");
		if ((j+1)%8 == 0) printf("\n");
	}
	printf("\n\n");
}



// SYMBOL TABLE

SymTab read_table_symboles(FILE * f, SecHead s, Elf_Header h)
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
		if (h.architecture == 1){
			//Check for type == SHT_REL or SHT_RELA
			if(c->t32.sh_type == SHT_REL || c->t32.sh_type == SHT_RELA)
			{
				//Get the index of the symbol table section header
				int index = c->t32.sh_link;

				for(int j=0 ; j<index ; j++)
					c2 = c2->suivant;

				st.nb = c2->t32.sh_size / 16;

				fseek(f, c2->t32.sh_offset, SEEK_SET);
					for(int j=0 ; j < st.nb ; j++)
					{
						current->t32.st_name = read_Elf32_Word(f, h.endianess);
						current->t32.st_value = read_Elf32_Addr(f, h.endianess);
						current->t32.st_size = read_Elf32_Word(f, h.endianess);
						current->t32.st_info = read_unsigned_char(f);
						current->t32.st_other = read_unsigned_char(f);
						current->t32.st_shndx = read_Elf32_Half(f, h.endianess);

						precedent = current;
						current = malloc (sizeof(OneSymbol));
						precedent->suivant = current;
					}
			}
		}
		else if(h.architecture ==2) {
			//Check for type == SHT_REL or SHT_RELA
			if(c->t64.sh_type == SHT_REL || c->t64.sh_type == SHT_RELA)
			{
				//Get the index of the symbol table section header
				int index = c->t64.sh_link;

				for(int j=0 ; j<index ; j++)
					c2 = c2->suivant;

				st.nb = c2->t64.sh_size / 16;

				fseek(f, c2->t64.sh_offset, SEEK_SET);
					for(int j=0 ; j < st.nb ; j++)
					{
						current->t64.st_name = read_Elf32_Word(f, h.endianess);
						current->t64.st_value = read_Elf32_Addr(f, h.endianess);
						current->t64.st_size = read_Elf32_Word(f, h.endianess);
						current->t64.st_info = read_unsigned_char(f);
						current->t64.st_other = read_unsigned_char(f);
						current->t64.st_shndx = read_Elf32_Half(f, h.endianess);

						precedent = current;
						current = malloc (sizeof(OneSymbol));
						precedent->suivant = current;
					}
			}
		}
		c = c->suivant;
		c2 = s.tete;
	}
	return st;
}

void print_table_symboles(SymTab st, StringTab string)
{
	printf("La table de symboles contient %d entrées :\n", st.nb);
	printf("[Nr]\tValue\t Size\t  Type\t\tBind\t   Vis\t    Ndx  Name\n");

	OneSymbol *current = st.tete;

	for(int i=0 ; i < st.nb ; i++)
	{
		printf("[%d]\t", i);
		printf("%08x ", current->t32.st_value);
		printf("%08x ", current->t32.st_size);
		int type = ELF32_ST_TYPE(current->t32.st_info);
		if(type == 0){printf("STT_NOTYPE\t");}else if(type == 1){printf("STT_OBJECT\t");}else if(type == 2){printf("STT_FUNC\t");}else if(type == 3){printf("STT_SECTION\t");}else if(type == 4){printf("STT_FILE\t");}else if(type == 13){printf("STT_LOPROC\t");}else if(type == 15){printf("STT_HIPROC\t");}else{printf("%x\t",type);}
		int bind = ELF32_ST_BIND(current->t32.st_info);
		if(bind == 0){printf("STB_LOCAL  ");}else if(bind == 1){printf("STB_GLOBAL ");}else if(bind == 2){printf("STB_WEAK\t");}else if(bind == 13){printf("STB_LOPROC\t");}else if(bind == 15){printf("STB_HIPROC\t");}else{printf("%02x\t   ",bind);}
		int other = current->t32.st_other;
		if(other == 0){printf("DEFAULT  ");}else{printf("%02x\t    ", other);}
		printf("%04x ", current->t32.st_shndx);
		print_string(string, current->t32.st_name);
		printf("\n");
		current = current->suivant;
	}
	printf("\n\n");
}



// REIMPLEMENTATION TABLE

ReimpTab read_table_reimplantation(FILE * f, SecHead s, SymTab st, Elf_Header h, int nb)
{
	OneHeader *c = s.tete;
	OneSymbol *sc = st.tete;

	ReimpTab r;
	r.tete = NULL;

	OneReimp *current, *precedent;
	current = malloc (sizeof(OneReimp));
	r.tete = current;

	int compt =0;
	int nbFinal = -1;


	for (int i = 0; i < s.nb; i++){
		if(h.architecture==1){
			// Chek type = SHT_REL ou RELA
			if(c->t32.sh_type == SHT_REL || c->t32.sh_type == SHT_RELA)
			{
				if (compt == nb)
				{
					r.nb = c->t32.sh_size / 8;
					r.offset = c->t32.sh_offset;

					fseek(f, c->t32.sh_offset ,SEEK_SET); //Offset

					for(int j=0 ; j < c->t32.sh_size / 8 ; j++) // Size
					{ 
						current->t32.r_offset = read_Elf32_Addr(f, h.endianess);
						current->t32.r_info = read_Elf32_Word(f, h.endianess);

						for (int x=0 ; x< ELF32_R_SYM(current->t32.r_info) ; x++)
							sc = sc->suivant;

						current->value = sc->t32.st_value;
						current->name = sc->t32.st_name;

						sc = st.tete;
						precedent = current;
						current = malloc (sizeof(OneReimp));
						precedent->suivant = current;

						compt++;
					}
					nbFinal = compt;
				}
				else compt++;
			}
		}
		else if (h.architecture==2){
			// Chek type = SHT_REL ou RELA
			if(c->t64.sh_type == SHT_REL || c->t64.sh_type == SHT_RELA)
			{
				if (compt == nb)
				{
					r.nb = c->t64.sh_size / 8;
					r.offset = c->t64.sh_offset;

					fseek(f, c->t64.sh_offset ,SEEK_SET); //Offset

					for(int j=0 ; j < c->t64.sh_size / 8 ; j++) // Size
					{ 
						current->t64.r_offset = read_Elf64_Addr(f, h.endianess);
						current->t64.r_info = read_Elf64_Word(f, h.endianess);

						for (int x=0 ; x < ELF32_R_SYM(current->t64.r_info) ; x++)
							sc = sc->suivant;

						current->value = sc->t64.st_value;
						current->name = sc->t64.st_name;

						sc = st.tete;
						precedent = current;
						current = malloc (sizeof(OneReimp));
						precedent->suivant = current;

						compt++;
					}
					nbFinal = compt;
				}
				else compt++;
			}
		}
		c = c->suivant;
	}

	if (nbFinal == -1) r.offset = -1;
	return r;
}

void print_table_reimp(ReimpTab r, StringTab string2, StringTab string1)
{
	printf("Section de réadressage '.rel.text' à l'adresse de décalage 0x%x contient %d entrées :\n", r.offset, r.nb);
	printf("[Nb]\tOffset   Info     Type     Val.-sym Noms-symboles\n");

	OneReimp *c = r.tete;

	for(int i=0 ; i < r.nb ; i++)
	{
		printf("[%d]\t", i);
		printf("%08x ", c->t32.r_offset);
		printf("%08x ", c->t32.r_info);
		printf("%08x ", ELF32_R_TYPE(c->t32.r_info));
		printf("%08x ", c->value);
		printf("%08x ", c->name);
		print_string(string2, c->name);
		printf("\n");

		c = c->suivant;
	}
	printf("\n\n");
}

ListReimpTab read_table_reimplantation_new(FILE * f, SecHead s, SymTab st, Elf_Header h)
{
	ListReimpTab LR;

	OneList *c, *p;
	c = malloc(sizeof(OneList));
	LR.tete = c;
	LR.nb = 0;

	ReimpTab r = read_table_reimplantation(f, s, st, h, 0);

	while (r.offset != -1)
	{
		c->r = r;
		LR.nb ++;
		
		r = read_table_reimplantation(f, s, st, h, LR.nb);
		
		p = c;
		c = malloc(sizeof(OneList));
		p->suivant = c;
	}

	return LR;
}

void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1)
{
	OneList *c = LR.tete;
	for(int i=0 ; i < LR.nb ; i++)
	{
		print_table_reimp (c->r, string2, string1);
		c = c->suivant;
	}
}

// STRING TABLE

StringTab read_string_table(FILE * f, Elf_Header head, SecHead s, int nb)
{
	OneHeader *c = s.tete;
	for (int i=0 ; i<nb ; i++)
	{
		if (c->t32.sh_type != SHT_STRTAB)
		{
			while(c->t32.sh_type != SHT_STRTAB)
				c = c->suivant;
		}
		else
		{
			c = c->suivant->suivant;
		}
	}


	fseek(f, c->t32.sh_offset ,SEEK_SET);

	StringTab string;
	string.nb = 0;

	OneString *current;
	OneString *precedent;

	char cc;

	current = malloc (sizeof(OneString));
	string.tete = current;

	for(int i=0 ; i<c->t32.sh_size ; i++)
	{
		cc = read_unsigned_char(f);
		current->c = cc;
		string.nb ++;

		precedent = current;
		current = malloc (sizeof(OneString));
		precedent->suivant = current;
	}
	return string;
}

void  print_string(StringTab string, int pos)
{
	OneString *current = string.tete;

	if(pos <= string.nb){
		for(int i = 0; i < pos; i++){
			current = current->suivant;
		}

		while(current->c != '\0'){			
			printf("%c", current->c);	
			current = current->suivant;
		}
	}
	printf("\t");
}



// UTILS

OFile initOFile(FILE * fich_o)
{
	OFile a;
	a.f = fich_o;
	a.h = read_header(fich_o);
	a.s = read_section_headers(fich_o, a.h);
	a.string1 = read_string_table(fich_o, a.h, a.s, 1);
	a.string2 = read_string_table(fich_o, a.h, a.s, 2);
	a.st = read_table_symboles(fich_o, a.s, a.h);
	a.LR = read_table_reimplantation_new(fich_o, a.s, a.st, a.h);
	return a;
}

void printOFile(OFile a)
{
	print_header(a.h);
	print_section_headers(a.s, a.string1);
	print_section(a.f, a.s, 1);
	print_table_symboles(a.st, a.string2);
	print_table_reimp_new(a.LR, a.string2, a.string1);
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
	
	OneList *cg = a.LR.tete;
	OneList *pg;
	while (cg != NULL)
	{
		pg = cg;
		cg = cg->suivant;
		
		OneReimp *c3 = pg->r.tete;
		OneReimp *p3;
		while(c3 != NULL)
		{
			p3 = c3;
			c3 = c3->suivant;	
			free(p3);
		}

		free(pg);
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

