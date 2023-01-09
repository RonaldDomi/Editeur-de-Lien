//
//  readAll.h
//  Editeur-de-lien
//

#include "CustomElf.h"

/* Alloue de la mémoire pour la création d'une structure regroupant:
	Un en-tête ELF
	Toutes les tables de sections d'un ELF
	Toutes les tables de symboles d'un ELF
	Tout le contenu d'un ELF
 
Paramètres : Aucun
Sortie : Retourne un pointeur vers la structure
*/
Elf32_Info * initElf32_Inf();


/* Remplit la structure contenant toutes les informations d'un ELF, ajoute donc les informations de :
	L'en-tête
	Les tables de sections
	Les tables de symboles
	Le contenu
 Paramètres : Un pointeur file de fichier
 Sortie : Retourne un pointeur vers la structure
 */
Elf32_Info * getAllInfo(FILE * file);
