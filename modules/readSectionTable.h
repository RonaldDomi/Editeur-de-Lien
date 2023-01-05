//
//  readSectionTable.h
//  Editeur de Liens
//

#ifndef readSectionTable_h
#define readSectionTable_h

#include <stdio.h>
#include "CustomElf.h"

Elf32_AllSec * initSectionTable(int nb);

void printAllSectionsTables(FILE * file, Elf32_AllSec * Sections, Elf32_Ehdr * Header);

/* Décode les octets des types de la section.

 Paramètre: un pointeur SectionTable de structure d'en-tête
 Sortie : Ne renvoie rien
 Effets de bords : Affiche les détails des flags du Header de la section
 */
void DecodeSectionType(Elf32_Shdr *SectionTable);

/* Décode les octets des flags de la section. Chaque décodage est un décalage bit-à-bit.

 Paramètre: un pointeur SectionTable de structure d'en-tête
 Sortie : Ne renvoie rien
 Effets de bords : Affiche les détails des flags du Header de la section
 */
void DecodeSectionFlags(Elf32_Shdr *SectionTable);

void printSectionTable(Elf32_Shdr *SectionTable);

/* Lit la valeur du nom de la table et la stocke dans la structure SectionTable
 Affiche la valeur suivant le paramètre verbose.
 La valeur du nom est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure SectionTable, ajoute la valeur à sh_name
 */
void sectionName(FILE *file, Elf32_Ehdr *Header, Elf32_Shdr *SectionTable, char nom);

/* Affiche le numéro de la section entre crochets aligné à droite
 Par exemple pour 101 sections à afficher: [  0] ... [ 10] ... [100]

 Paramètre: un pointeur Header de structure d'en-tête, un entier correspondant au numéro de la section à afficher.
 Sortie : Ne renvoie rien
 Effets de bords : Affiche le numéro de la section, entre crochets, aligné à droite
 */
void printNumber(int nbSections, int sectionNumber);

/* Remplit la structure SectionTable

 Paramètre:
	un pointeur file de fichier,
	un pointeur SectionTable de structure d'en-tête de section,
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure SectionTable, la remplie entièrement
*/
void getSectionTable(FILE *file, Elf32_Shdr *SectionTable);

/* Remplit la structure Sections contenant l'ensemble des Tables de Sections
 Paramètre:
 un pointeur file de fichier,
 un pointeur Header de structure d'en-tête,
 un pointeur Sections de structure contenant l'ensemble des Tables de Sections
 Effets de bords : Modifie la structure SectionTable, la remplie entièrement
 */
void getAllSectionsTables(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec *Sections);

/* Chereche si une fonction d'un certain nom existe et renvoit son index
 Paramètre:
 un pointeur Header de structure d'en-tête,
 un pointeur Sections de structure contenant l'ensemble des Tables de Sections
 un pointeur char désignant spécifiquement le nom de section à chercher
 Sortie : Renvoie i l'index de la section si elle a été trouvée
			-1 Sinon
 Effets de bords : Modifie la structure SectionTable, la remplie entièrement
 */
int getSectionByName(Elf32_AllSec *Sections, char *sectionName);

/* Chereche si une fonction d'un certain type existe et renvoit son index
 Paramètre:
 un pointeur Header de structure d'en-tête,
 un pointeur Sections de structure contenant l'ensemble des Tables de Sections
 un pointeur char désignant spécifiquement le nom de section à chercher
 Sortie : Renvoie i l'index de la section si elle a été trouvée
			-1 Sinon
 Effets de bords : Modifie la structure SectionTable, la remplie entièrement
 */
int getSectionByType(Elf32_AllSec *Sections, int type);

#endif /* readSectionTable_h */
