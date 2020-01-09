#ifndef __FUSION_H__
#define __FUSION_H__

#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "utils.h"
#include "es.h"

/*
   fusion
   description : fusionne deux Ofile en un seul
   parametres : un Ofile a et un Ofile b qui seront fusionné et un Ofile de destination
   valeur de retour : aucun
   effets de bord : modifie le Ofile de destination
*/
void fusion(OFile a, OFile b, OFile dest);

/*
   write_header
   description : écris le header du Ofile en paramètre dans son fichier
   parametres : un Ofile
   valeur de retour : aucun
   effets de bord : aucun
*/
void write_header(OFile dest);

/*
   write_section
   description : écris les sections du Ofile source dans le fichier du Ofile dest
   parametres : Un Ofile source, un Ofile dest et un header de section
   valeur de retour : aucun
   effets de bord : aucun
*/
void write_section(OFile source, OFile dest, OneHeader *secHeader);

/*
   write_section_header
   description : écris le header de section du Ofile en paramètre
   parametres : un Ofile
   valeur de retour : aucun
   effets de bord : aucun
*/
void write_section_header(OFile dest);

/*
   fusion_table_symbole
   description : fusionne la table des symbole dus Ofile a et b dans le Ofile dest
   parametres : un Ofile a et un Ofile b dont la table sera fusionné dans un Ofile dest
   valeur de retour : aucun
   effets de bord : modifie le Ofile dest
*/
void fusion_table_symbole(OFile a,OFile b,OFile dest);

/*
   comparaison
   description : fusionne deux symboles dans une table de destination
   parametres : Une table de symbole, le symbole courant, le dernier symbole de destination et l'architecture
   valeur de retour : 0 si non ajouté et 1 si ajouté
   effets de bord : modifie la table de symbole de destination
*/
int comparaison(SymTab ts1,OneSymbol *current, OneSymbol *previous, OneSymbol *endDestSymTab, int architecture);
#endif
