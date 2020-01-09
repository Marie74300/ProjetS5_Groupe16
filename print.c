#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "read_func.h"
#include "read.h"
#include "print.h"

void print_header(Elf_Header head)
{
	printf("\nELF Header:\n");

		//DEBUT de l'affichage du header
		printf("  Magic:   ");
		for(int i=0 ; i<EI_NIDENT ; i++)
			printf("%02x ", head.headformat.e_ident[i]);
		printf("\n");

		printf("  Class:                             ");
		//traitement des differents Class pour signification
		if(head.headformat.e_ident[EI_CLASS]==0){printf("ELFCLASSNONE\n");}else if(head.headformat.e_ident[EI_CLASS]==1){printf("ELFCLASS32\n");}else if(head.headformat.e_ident[EI_CLASS]==2){printf("ELFCLASS64\n");}else{printf("%hd\n", head.headformat.e_ident[EI_CLASS]);}
		
		printf("  Data:                              ");
		//traitement des differents Data pour signification
		if(head.headformat.e_ident[EI_DATA]==0){printf("ELFDATANONE\n");}else if(head.headformat.e_ident[EI_DATA]==1){printf("ELFDATA2LSB\n");}else if(head.headformat.e_ident[EI_DATA]==2){printf("ELFDATA2MSB\n");}else{printf("%hd\n", head.headformat.e_ident[EI_DATA]);}
		
		printf("  Version:                           ");
		//traitement des differents Version pour signification
		if(head.headformat.e_ident[EI_VERSION]==0){printf("EV_NONE\n");}else if(head.headformat.e_ident[EI_VERSION]==1){printf("EV_CURRENT\n");}else{printf("%hd\n", head.headformat.e_ident[EI_VERSION]);}
	//CAS 32 BIT
	if (head.architecture == 1){
			printf("  OS/ABI:                            ");
			//traitement des differents OS/ABI pour signification
			if(head.headformat.e_ident[EI_OSABI]==0x00){printf("UNIX - Systeme V\n");}else if(head.headformat.e_ident[EI_OSABI]==0x03){printf("Linux\n");}else if(head.headformat.e_ident[EI_OSABI]==0x06){printf("Solaris\n");}else if(head.headformat.e_ident[EI_OSABI]==0x0A){printf("UNIX - True64\n");}else{printf("%hd\n", head.headformat.e_ident[EI_OSABI]);}
	}
	//CAS 64 BIT
	else if (head.architecture == 2){
		printf("  OS/ABI:                            ");
		printf("%hd\n", head.headformat.e_ident[EI_OSABI]);
	}
		printf("  ABI Version:                       ");
		printf("%hd\n", head.headformat.e_ident[EI_ABIVERSION]);

		printf("  Type:                              ");
		//traitement des differents Type pour signification
		if(head.headformat.e_type==0){printf("ET_NONE\n");}else if(head.headformat.e_type==1){printf("ET_REL\n");}else if(head.headformat.e_type==2){printf("ET_EXEC\n");}else if(head.headformat.e_type==3){printf("ET_DYN\n");}else if(head.headformat.e_type==4){printf("ET_CORE\n");}else if(head.headformat.e_type==0xff00){printf("ET_LOPROC\n");}else if(head.headformat.e_type==0xffff){printf("ET_HIPROC\n");}else{printf("\n");}

		printf("  Machine:                           ");
		if(head.headformat.e_machine==40){printf("ARM\n");}else{printf("\n");}

		printf("  Version:                           ");
		printf("0x%x\n", head.headformat.e_version);
		//Cas 32 Bits
		if (head.architecture == 1){
			printf("  Entry point address:               ");
			//0xffffffff a etait rajoute pour traiter les 32 bit comme 64
			printf("0x%lx\n", head.headformat.e_entry & 0xffffffff);

			printf("  Start of program headers:          ");
			printf("%ld (bytes into file)\n", head.headformat.e_phoff & 0xffffffff);

			printf("  Start of section headers:          ");
			printf("%ld (bytes into file)\n", head.headformat.e_shoff & 0xffffffff);
		}
		//Cas 64 Bits
		else if (head.architecture == 2){
			printf("  Entry point address:               ");
			printf("0x%lx\n", head.headformat.e_entry);

			printf("  Start of program headers:          ");
			printf("%ld (bytes into file)\n", head.headformat.e_phoff);

			printf("  Start of section headers:          ");
			printf("%ld (bytes into file)\n", head.headformat.e_shoff);

		}
		printf("  Flags:                             ");
		printf("%hd\n", head.headformat.e_flags);

		printf("  Size of this header:               ");
		printf("%hd (bytes)\n", head.headformat.e_ehsize);

		printf("  Size of program headers:           ");
		printf("%hd (bytes)\n", head.headformat.e_phentsize);

		printf("  Number of program headers:         ");
		printf("%hd\n", head.headformat.e_phnum);

		printf("  Size of section headers:           ");
		printf("%hd (bytes)\n", head.headformat.e_shentsize);

		printf("  Number of section headers:         ");
		printf("%hd\n", head.headformat.e_shnum);

		printf("  Section header string table index: ");
		printf("%hd\n\n", head.headformat.e_shstrndx);
	
}


void print_section_headers(SecHead section, StringTab string)
{
	//DEBUT AFFICHAGE SECTION HEADER
	printf("Section Headers:\n");
	printf("[Nr]\tName\t\tType\t\tAddr\tOff\tSize\tES\tFlg\tLk\tInf\tAl\n");

	OneHeader *current_head = section.tete;
	//Tant que l'on a pas affiché toute les sections 
	for(int i=0 ; i<section.nb ; i++)
	{
		printf("[%d]\t", i);

		//printf("%x\t", c->t.sh_name);
		print_string(string, current_head->tableformat.sh_name);
		if(i!=2 && i!=5 && i!=6){printf("\t");}
		//traitement des different types
		if(current_head->tableformat.sh_type==0){printf("SHT_NULL\t");}else if(current_head->tableformat.sh_type==1){printf("SHT_PROGBITS\t");}else if(current_head->tableformat.sh_type==2){printf("SHT_SYMTAB\t");}else if(current_head->tableformat.sh_type==3){printf("SHT_STRTAB\t");}else if(current_head->tableformat.sh_type==4){printf("SHT_RELA\t\t");}else if(current_head->tableformat.sh_type==5){printf("SHT_HASH\t");}else if(current_head->tableformat.sh_type==6){printf("SHT_DYNAMIC\t");}else if(current_head->tableformat.sh_type==7){printf("SHT_NOTE\t");}else if(current_head->tableformat.sh_type==8){printf("SHT_NOBITS\t");}else if(current_head->tableformat.sh_type==9){printf("SHT_REL\t\t");}else if(current_head->tableformat.sh_type==10){printf("SHT_SHLIB\t");}else if(current_head->tableformat.sh_type==11){printf("SHT_DYNSYM\t");}else if(current_head->tableformat.sh_type==0x70000000){printf("SHT_LOPROC\t");}else if(current_head->tableformat.sh_type==0x7fffffff){printf("SHT_HIPROC\t");}else if(current_head->tableformat.sh_type==0x80000000){printf("SHT_LOUSER\t");}else if(current_head->tableformat.sh_type==0xffffffff){printf("SHT_HIUSER\t");}else if(current_head->tableformat.sh_type==0x70000003){printf("ARM_ATTRIBUTES\t");}else{if(i==5){printf("%x\t", current_head->tableformat.sh_type);}else{printf("%x\t\t", current_head->tableformat.sh_type);}}
		//affichage sans difference 32 / 64 Bit
 		printf("%lx\t", current_head->tableformat.sh_addr & 0xffffffff);
		printf("%lx\t", current_head->tableformat.sh_offset & 0xffffffff);
		printf("%lx\t", current_head->tableformat.sh_size & 0xffffffff);
		printf("%lx\t", current_head->tableformat.sh_entsize & 0xffffffff);
		printf("%lx\t", current_head->tableformat.sh_flags & 0xffffffff);
		printf("%x \t", current_head->tableformat.sh_link);
		printf("%x \t", current_head->tableformat.sh_info);
		printf("%lx\n", current_head->tableformat.sh_addralign & 0xffffffff);
		//passage à la suite
		current_head = current_head->suivant;
	}
	printf("\n");

}

//fonction permettant d'afficher lles caracteres
void  print_string(StringTab string, int pos)
{
	OneString *current = string.tete;
	//si on a encore des nom non parcourus
	if(pos <= string.nb){
		//tant que l'on n'est pas des la partie que l'on veux afficher
		for(int i = 0; i < pos; i++){
			current = current->suivant;
		}
		//affichage caractère pas caractère
		while(current->c != '\0'){			
			printf("%c", current->c);	
			current = current->suivant;
		}
	}
	printf("\t");
}

//affichage table symbole
void print_table_symboles(SymTab symb_table, StringTab string)
{
	//DEBUT de l'affichage
	printf("La table de symboles contient %d entrées :\n", symb_table.nb);
	printf("[Nr]\tValue\t Size\t  Type\t\tBind\t   Vis\t    Ndx  Name\n");

	OneSymbol *current = symb_table.tete;

	//tant que l'on a pas tout afficher
	for(int i=0 ; i < symb_table.nb ; i++)
	{	
		//affichage
		printf("[%d]\t", i);
		printf("%08lx ", current->tableformat.st_value & 0xffffffff);
		printf("%08lx ", current->tableformat.st_size & 0xffffffff);
		//traitement des differents TYPE
		int type = ELF32_ST_TYPE(current->tableformat.st_info);
		if(type == 0){printf("STT_NOTYPE\t");}else if(type == 1){printf("STT_OBJECT\t");}else if(type == 2){printf("STT_FUNC\t");}else if(type == 3){printf("STT_SECTION\t");}else if(type == 4){printf("STT_FILE\t");}else if(type == 13){printf("STT_LOPROC\t");}else if(type == 15){printf("STT_HIPROC\t");}else{printf("%x\t",type);}
		//traitement des differents BIND
		int bind = ELF32_ST_BIND(current->tableformat.st_info);
		if(bind == 0){printf("STB_LOCAL  ");}else if(bind == 1){printf("STB_GLOBAL ");}else if(bind == 2){printf("STB_WEAK\t");}else if(bind == 13){printf("STB_LOPROC\t");}else if(bind == 15){printf("STB_HIPROC\t");}else{printf("%02x\t   ",bind);}
		//traitement des different other (si egal 0 ou non
		int other = current->tableformat.st_other;
		if(other == 0){printf("DEFAULT  ");}else{printf("%02x\t    ", other);}
		
		printf("%04x ", current->tableformat.st_shndx);
		print_string(string, current->tableformat.st_name);
		printf("\n");
		//passage au suivant
		current = current->suivant;
	}
	printf("\n\n");
}


// SECTION

void print_section(FILE * f, SecHead section, int i)
{
	//DEBUT de l'affichage
	printf("Affichage de la section %d:\n", i);

	OneHeader *current_head = section.tete;
	//tant que l'on n'est pas dans la section
	for(int j=0 ; j<i ; j++){
		current_head = current_head->suivant;
		if(current_head->suivant == NULL){
			printf("Numéro de section invalide : Max = %d\n", j);
			// Affichage de la section par défaut
			print_section(f, section, 1);
			return;
		}
	}
	//placement dans la section 
	fseek(f, current_head->tableformat.sh_offset ,SEEK_SET);
	//affichage
	for(int j=0 ; j < current_head->tableformat.sh_size ; j++)
	{
		printf("%02x ", read_unsigned_char(f));
		if ((j+1)%4 == 0) printf("\t");
		if ((j+1)%8 == 0) printf("\n");
	}
	printf("\n\n");
}

void print_table_reimp(OneList * reimp, StringTab string2, StringTab string1)
{
	ReimpTab r = reimp->r;	

	//Debut d'affichage
	printf("Section de réadressage '%s' à l'adresse de décalage 0x%x contient %d entrées :\n", reimp->name, r.offset, r.nb);
	printf("[Nb]\tOffset   Info     Type     Val.-sym Noms-symboles\n");

	OneReimp *current_reimp = r.tete;
	
	//tant que l'on a pas affiche toute la table
	for(int i=0 ; i < r.nb ; i++)
	{
		printf("[%d]\t", i);
		printf("%08lx ", current_reimp->tableformat.r_offset & 0xffffffff);
		printf("%08lx ", current_reimp->tableformat.r_info & 0xffffffff);
		printf("%08lx ", ELF32_R_TYPE(current_reimp->tableformat.r_info) & 0xffffffff);
		printf("%08x ", current_reimp->value);
		printf("%08x ", current_reimp->name);
		print_string(string2, current_reimp->name);
		printf("\n");
		//passage a la suite de la table
		current_reimp = current_reimp->suivant;
	}
	printf("\n\n");
}


//fonction appellant la fonction print_table_réimp
void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1)
{
	OneList *current_List = LR.tete;
	//tant que l'on a pas parcourus tout les reimplementation 
	for(int i=0 ; i < LR.nb ; i++)
	{
		print_table_reimp (current_List, string2, string1);
		current_List = current_List->suivant;
	}
}


