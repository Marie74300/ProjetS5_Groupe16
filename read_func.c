#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

#include "read_func.h"
#include "read.h"
#include "print.h"

int power(int entier1, int entier2)
{
	int puissance=1;
	for (int indice=0 ; indice<entier2 ; indice++)
		puissance *= entier1;
	return puissance;
}



// HEADER

Elf_Header read_header(FILE * f)
{
	//initialisation des variables
	Elf_Header head;
	int tmp;

	//insertion de tout les données dans tableau 
	for (int i=0 ; i<EI_NIDENT ; i++)
	{
		tmp = read_unsigned_char(f);
		head.headformat.e_ident[i] = tmp;
	}
	//initialisation de la fin des données
	int endianess = head.headformat.e_ident[EI_DATA];
	head.endianess = endianess;
	//initialisation de toute les donnée dans leur variable respective avec le mode de lecture approprié
	head.headformat.e_type = read_Elf32_Half(f, endianess);
	head.headformat.e_machine = read_Elf32_Half(f, endianess);
	head.headformat.e_version = read_Elf32_Word(f, endianess);
	//cas 32 bits
	if (head.headformat.e_ident[EI_CLASS] == 1)
	{
		head.architecture = 1;
		head.headformat.e_entry = read_Elf32_Addr(f, endianess);
		head.headformat.e_phoff = read_Elf32_Off(f, endianess);
		head.headformat.e_shoff = read_Elf32_Off(f, endianess);
		head.headformat.e_flags = read_Elf32_Word(f, endianess);
	}
	//cas 64 bits
	else if (head.headformat.e_ident[EI_CLASS] == 2)
	{
		head.architecture = 2;
		

		head.headformat.e_entry = read_Elf64_Addr(f, endianess);
		head.headformat.e_phoff = read_Elf64_Off(f, endianess);
		head.headformat.e_shoff = read_Elf64_Off(f, endianess);
		head.headformat.e_flags = read_Elf32_Off(f, endianess);
	}
	head.headformat.e_ehsize = read_Elf32_Half(f, endianess);
	head.headformat.e_phentsize = read_Elf32_Half(f, endianess);
	head.headformat.e_phnum = read_Elf32_Half(f, endianess);
	head.headformat.e_shentsize = read_Elf32_Half(f, endianess);

	head.headformat.e_shnum = read_Elf32_Half(f, endianess);
	head.headformat.e_shstrndx = read_Elf32_Half(f, endianess);
	

	return head;
}




// SECTION HEADER

SecHead read_section_headers(FILE * f, Elf_Header head)
{
	//initialisation de variables
	int e_shoff =  head.headformat.e_shoff;
	int e_shnum =  head.headformat.e_shnum;

	SecHead section;
	section.nb = e_shnum;
	section.tete = NULL;
	//placement dans le fichier selon e_shoff
	fseek(f, e_shoff ,SEEK_SET);

	OneHeader *current, *precedent;
	current = malloc (sizeof(OneHeader));
	section.tete = current;
		//insertion de chaque element de section dans leurs variables respectivent celon les données (e_shnum) 
		for(int i=0 ; i<e_shnum ; i++)
		{
			current->tableformat.sh_name = read_Elf32_Word(f, head.endianess);
			current->tableformat.sh_type = read_Elf32_Word(f, head.endianess);
			//cas 32 bits
			if (head.architecture == 1){
				current->tableformat.sh_flags = read_Elf32_Word(f, head.endianess);
				current->tableformat.sh_addr = read_Elf32_Addr(f, head.endianess);
			}
			//cas 64 bits
			else if (head.architecture==2) {
				current->tableformat.sh_flags = read_Elf64_Word(f, head.endianess);
				current->tableformat.sh_addr = read_Elf64_Addr(f, head.endianess);
			}
			
			current->tableformat.sh_offset = read_Elf32_Off(f, head.endianess);
			current->tableformat.sh_size = read_Elf32_Word(f, head.endianess);
			current->tableformat.sh_link = read_Elf32_Word(f, head.endianess);
			current->tableformat.sh_info = read_Elf32_Word(f, head.endianess);
			//cas 32 bits
			if (head.architecture == 1){
				current->tableformat.sh_addralign = read_Elf32_Word(f, head.endianess);
				current->tableformat.sh_entsize = read_Elf32_Word(f, head.endianess);
			}
			//cas 64 bits
			else if (head.architecture==2) {
				current->tableformat.sh_addralign = read_Elf64_Word(f, head.endianess);
				current->tableformat.sh_entsize = read_Elf64_Word(f, head.endianess);
			}
			//changement de donnée à traité et stockage
			precedent = current;
			current = malloc (sizeof(OneHeader));
			precedent->suivant = current;
		}

	return section;
}





// SYMBOL TABLE

SymTab read_table_symboles(FILE * f, SecHead section, Elf_Header head)
{
	//initialisation des variables 
	SymTab symbole_table;
	symbole_table.tete = NULL;

	OneSymbol *current, *precedent;
	current = malloc (sizeof(OneSymbol));
	symbole_table.tete = current;

	OneHeader *c = section.tete;
	OneHeader *c2 = section.tete;
	//insertion des symbole selon leurs variables respectivent par rapport au nombe de section 
	for (int i = 0; i < section.nb; i++) 
	{
			//Check si type == SHT_REL or SHT_RELA
			if(c->tableformat.sh_type == SHT_REL || c->tableformat.sh_type == SHT_RELA)
			{
				//Get the index of the symbol table section header
				int index = c->tableformat.sh_link;
				//passe les parties inutiles
				for(int j=0 ; j<index ; j++)
					c2 = c2->suivant;

				symbole_table.nb = c2->tableformat.sh_size / 16;
				//placement dans le fichier 
				fseek(f, c2->tableformat.sh_offset, SEEK_SET);
					for(int j=0 ; j < symbole_table.nb ; j++)
					{
						current->tableformat.st_name = read_Elf32_Word(f, head.endianess);
						current->tableformat.st_value = read_Elf32_Addr(f, head.endianess);
						current->tableformat.st_size = read_Elf32_Word(f, head.endianess);
						current->tableformat.st_info = read_unsigned_char(f);
						current->tableformat.st_other = read_unsigned_char(f);
						current->tableformat.st_shndx = read_Elf32_Half(f, head.endianess);
						//changement de donnée à traité et stockage
						precedent = current;
						current = malloc (sizeof(OneSymbol));
						precedent->suivant = current;
					}
			}
		
		c = c->suivant;
		c2 = section.tete;
	}
	return symbole_table;
}





// REIMPLEMENTATION TABLE

ReimpTab read_table_reimplantation(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head, int nb)
{
	
	OneHeader *current_head = section.tete;
	OneSymbol *symb_curent = symbole_table.tete;

	ReimpTab r;
	r.tete= NULL;

	OneReimp *current, *precedent;
	current = malloc (sizeof(OneReimp));
	r.tete = current;

	int compt =0;
	int nbFinal = -1;


	for (int i = 0; i < section.nb; i++){
		
		// Chek type = SHT_REL ou RELA
		if(current_head->tableformat.sh_type == SHT_REL || current_head->tableformat.sh_type == SHT_RELA)
		{
			if (compt == nb)
			{
				r.nb = current_head->tableformat.sh_size / 8;
				r.offset = current_head->tableformat.sh_offset;

				fseek(f, current_head->tableformat.sh_offset ,SEEK_SET); //Offset

				for(int j=0 ; j < current_head->tableformat.sh_size / 8 ; j++) // Size
				{ 
					if(head.architecture==1){
						current->tableformat.r_offset = read_Elf32_Addr(f, head.endianess);
						current->tableformat.r_info = read_Elf32_Word(f, head.endianess);
					}
					else if (head.architecture==2){

						current->tableformat.r_offset = read_Elf64_Addr(f, head.endianess);
						current->tableformat.r_info = read_Elf64_Word(f, head.endianess);
					}
					
					for (int x=0 ; x< ELF32_R_SYM(current->tableformat.r_info) ; x++)
						symb_curent = symb_curent->suivant;

					current->value = symb_curent->tableformat.st_value;
					current->name = symb_curent->tableformat.st_name;

					symb_curent = symbole_table.tete;
					precedent = current;
					current = malloc (sizeof(OneReimp));
					precedent->suivant = current;

					compt++;
				}
				nbFinal ++;
			}
			else compt++;
		}

		current_head = current_head->suivant;
	}
	
	
	if (nbFinal == -1){ 
		r.offset = -1;
	}
	return r;
}



ListReimpTab read_table_reimplantation_new(FILE * f, SecHead section, SymTab symbole_table, Elf_Header head)
{
	ListReimpTab LR;

	OneList *current, *precedent;
	current = malloc(sizeof(OneList));
	LR.tete = current;
	LR.nb = 0;

	ReimpTab r = read_table_reimplantation(f, section, symbole_table, head, 0);

	while (r.offset != -1)
	{
		current->r = r;
		LR.nb ++;
		
		r = read_table_reimplantation(f, section, symbole_table, head, LR.nb);
		
		precedent = current;
		current = malloc(sizeof(OneList));
		precedent->suivant = current;
	}

	return LR;
}



// STRING TABLE

StringTab read_string_table(FILE * f, Elf_Header head, SecHead section, int nb)
{
	OneHeader *current_head = section.tete;
	for (int i=0 ; i<nb ; i++)
	{
		if (current_head->tableformat.sh_type != SHT_STRTAB)
		{
			while(current_head->tableformat.sh_type != SHT_STRTAB)
				current_head = current_head->suivant;
		}
		else
		{
			current_head = current_head->suivant->suivant;
		}
	}


	fseek(f, current_head->tableformat.sh_offset ,SEEK_SET);

	StringTab string;
	string.nb = 0;

	OneString *current;
	OneString *precedent;

	char tmp;

	current = malloc (sizeof(OneString));
	string.tete = current;

	for(int i=0 ; i<current_head->tableformat.sh_size ; i++)
	{
		tmp = read_unsigned_char(f);
		current->c = tmp;
		string.nb ++;

		precedent = current;
		current = malloc (sizeof(OneString));
		precedent->suivant = current;
	}
	return string;
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
	OneHeader *current1 = a.s.tete;
	OneHeader *precedent1;
	while(current1 != NULL)
	{
		precedent1 = current1;
		current1 = current1->suivant;	
		free(precedent1);
	}

	OneSymbol *current2 = a.st.tete;
	OneSymbol *precedent2;
	while(current2 != NULL)
	{
		precedent2 = current2;
		current2 = current2->suivant;	
		free(precedent2);
	}

	OneList *currentg = a.LR.tete;
	OneList *precedentg;
	while (currentg != NULL)
	{
		precedentg = currentg;
		currentg = currentg->suivant;
		
		OneReimp *current3 = precedentg->r.tete;
		OneReimp *precedent3;
		while(current3 != NULL)
		{
			precedent3 = current3;
			current3 = current3->suivant;	
			free(precedent3);
		}

		free(precedentg);
	}

	OneString *current4 = a.string1.tete;
	OneString *precedent4;
	while(current4 != NULL)
	{
		precedent4 = current4;
		current4 = current4->suivant;	
		free(precedent4);
	}

	OneString *current5 = a.string2.tete;
	OneString *precedent5;
	while(current5 != NULL)
	{
		precedent5 = current5;
		current5 = current5->suivant;	
		free(precedent5);
	}
}

