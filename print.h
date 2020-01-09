#ifndef __PRINT_H__
#define __PRINT_H__




void print_header(Elf_Header head);
/****************************************************************************************************************************************
print_header
Description: Affichages des données de Elf_Header (rempli par la fonction read_header) 
Paramètre  : Elf_Header head 
valeur de retour: aucune
effet de bord: Affichage complet du header
-Class    -Data     -Version    -OS/ABI    -ABI Version    -Type     -Machine	-...

****************************************************************************************************************************************/


void print_section_headers(SecHead section, StringTab string);
/****************************************************************************************************************************************
print_section_header
Description: Affichages des données de Elf_Header (rempli par la fonction read_header) 
Paramètre  : Elf_Header head 
valeur de retour: aucune
effet de bord: Affichage complet du header
-Class    -Data     -Version    -OS/ABI    -ABI Version    -Type     -Machine	-...

****************************************************************************************************************************************/


void print_string(StringTab string, int pos);


void print_table_symboles(SymTab st, StringTab string);


void print_section(FILE * f, SecHead s, int i);


void print_table_reimp(ReimpTab r, StringTab string2, StringTab string1);


void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1);

#endif
