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
Description: Affichages des section de SecHead (rempli par la fonction read_section_headers) 
Paramètre  : SecHead section StringTab string
valeur de retour: aucune
effet de bord: Affichage des informations des sections
-Nr	-name		-...

****************************************************************************************************************************************/


void print_string(StringTab string, int pos);
/****************************************************************************************************************************************
print_string
Description: Affichages caractere
Paramètre  : entier pos et StringTab sting (pour position et nombre)
valeur de retour: aucune
effet de bord: Afiichage des caracteres celon la position et la partie que l'on nous demande 

****************************************************************************************************************************************/



void print_table_symboles(SymTab st, StringTab string);
/****************************************************************************************************************************************
print_table_symbole
Description: Affichages des symboles SymTab st (rempli par la fonction read_symbole_table) 
Paramètre  : SymTab st, StringTab string
valeur de retour: aucune
effet de bord: Affichage complet des symboles
-Nr	-Value		-size 		-...

****************************************************************************************************************************************/


void print_section(FILE * f, SecHead s, int i);
/****************************************************************************************************************************************
print_section
Description: affiche le contenue des section
Paramètre  : FILE * f, SecHead s, int i
valeur de retour: aucune
effet de bord: affiche le contenu de la section en hexa

****************************************************************************************************************************************/



void print_table_reimp(OneList * reimp, StringTab string2, StringTab string1);
/****************************************************************************************************************************************
print_table_reimp
Description: affiche chaque element de r 
Paramètre  : ListReimpTab LR, StringTab string2, StringTab string1
valeur de retour: aucune
effet de bord: Affichage complet des information de replementation
-nb	-offset		-...

****************************************************************************************************************************************/



void print_table_reimp_new(ListReimpTab LR, StringTab string2, StringTab string1);
/****************************************************************************************************************************************
print_table_reimp_new
Description: appel print_table_reimp pour afficher tableau de chaque element de LR
Paramètre  : ListReimpTab LR, StringTab string2, StringTab string1
valeur de retour: aucune
effet de bord: appel une fonction pour affichage

****************************************************************************************************************************************/


#endif
