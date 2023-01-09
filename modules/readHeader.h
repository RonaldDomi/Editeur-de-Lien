//
//  readHeader.h
//  Editeur de Liens
//

#ifndef readHeader_h
#define readHeader_h

#include <stdio.h>
#include "CustomElf.h"

/* Décode les octets des flags du header. Chaque décodage est un décalage bit-à-bit.

 Paramètre: un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Affiche les détails des flags du Header
 */
void decodeHeaderFlags(Elf32_Ehdr *Header);

/* Affiche le header à partir de la structure Elf32_Ehdr

 Paramètre: un pointeur Header vers le type Elf32_Ehdr contenant tout les informations du Header
 Sortie : Ne renvoie rien
 */
void printHeader(Elf32_Ehdr *Header);

/* Remplit la structure Header 

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, la remplie entièrement
 */
void getHeader(FILE *file, Elf32_Ehdr *Header);

#endif /* readHeader_h */
