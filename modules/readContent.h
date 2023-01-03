//
//  readContent.h
//  Editeur-de-lien
//
#ifndef readContent_h
#define readContent_h

#include <stdio.h>
#include "CustomElf.h"

/* Affiche le contenu d'une section selectionnée par numéro ou par nom et son adresse de départ.
 L'affichage est similaire à readelf -x mais sans le décodage des données (qui n'est pas très pertinent de toute façon).

 Paramètres:
 Un pointeur file de fichier
 Un pointeur SectionTable d'en-tête de fichier
 Un entier désignant le numéro de la section selectionnée (-1 si on selectionne par nom)
 Un pointeur vers un char désignant le nom de la section selectionnée (NULL si on sélectionne par numéro)
 Sortie : Ne renvoie rien
 Effets de bords: Ecrit les données sur la sortie standard
 */
void printContent(FILE *file, Elf32_Shdr *SectionTable, int sectionSelected, char *nameOfSection);

#endif /* readContent_h */
