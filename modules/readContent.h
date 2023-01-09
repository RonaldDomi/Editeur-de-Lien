//
//  readContent.h
//  Editeur de Liens
//

#ifndef readContent_h
#define readContent_h

#include <stdio.h>
#include "CustomElf.h"

/* Lit dans le fichier, recupère tous les contenus de toutes les tables de sections et remplit la structure associée

 Paramètres: Un pointeur file de fichier
			Un pointeur SectionsTables vers toutes les tables de sections
 Sortie : Ne renvoie rien
 Effets de bords: Alloue de la mémoire
 */
void getAllSectionsContent(FILE* file, Elf32_AllSec * SectionsTables);


/* Affiche le contenu d'une section selectionnée par numéro et son adresse de départ.
 L'affichage est similaire à readelf -x mais sans le décodage des données (qui n'est pas très pertinent de toute façon).

 Paramètres: Un pointeur SectionTables des en-têtes de tables de sections
			Un entier désignant le numéro de la section selectionnée
 Sortie : Ne renvoie rien
 Effets de bords: Ecrit le contenu sur la sortie standard
 */
void printContent(Elf32_AllSec *SectionsTables, int sectionSelected);


#endif /* readContent_h */
