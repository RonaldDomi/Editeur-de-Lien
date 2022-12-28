//
//  readSectionTable.h
//  Editeur de Liens
//

#ifndef readSectionTable_h
#define readSectionTable_h

#include <stdio.h>
#include "CustomElf.h"

/* Lit la valeur du nom de la table et la stocke dans la structure SectionTable
 Affiche la valeur suivant le paramètre verbose.
 La valeur du nom est un Word, on lit donc 4 octets.
 === ATTENTION: Le détail n'est pas (encore) disponible pour cette caractéristique, bien qu'il serait pratique...
 Pour l'instant on affiche donc que la valeur qui n'est pas parlante par elle-même ===

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure SectionTable, ajoute la valeur à sh_name
 */
void sectionName(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);


/* Lit le type de la section et la stocke dans la structure SectionTable
 Affiche la valeur suivant le paramètre verbose.
 Le type est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure SectionTable, ajoute la valeur à sh_type
 */
void sectionType(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit les flags de la section et la stocke dans la structure SectionTable
 Affiche la valeur suivant le paramètre verbose.
 Les flags est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie: Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la valeur à sh_flags
 */
void sectionFlags(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit l'adresse du début de la section et la stocke dans la structure SectionTable
 Affiche l'adresse suivant le paramètre verbose.
 L'adresse est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la valeur à sh_addr
 */
void sectionAdress(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit le décalage la section et la stocke dans la structure SectionTable
 Affiche le décalage suivant le paramètre verbose.
 Le décalage est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la valeur à sh_offset
 */
void sectionOffset(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit la taille de la section et la stocke dans la structure SectionTable
 Affiche la taille suivant le paramètre verbose.
 La taille est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la valeur à sh_size
 */
void sectionSize(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit l'index de lien la section et la stocke dans la structure SectionTable
 Affiche l'index suivant le paramètre verbose.
 L'index est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute l'index à sh_link
 */
void sectionLink(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit les informations additionnelles la section et la stocke dans la structure SectionTable
 Affiche les infos suivant le paramètre verbose.
 Les infos est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la valeur à sh_info
 */
void sectionInfo(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit l'alignement de l'adresse la section et la stocke dans la structure SectionTable
 Affiche l'alignement suivant le paramètre verbose.
 L'alignement est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute l'adresse à sh_addralign
 */
void sectionAdressAlign(FILE *file, Elf32_Shdr* SectionTable, char verbose);


/* Lit la taille de l'entrée de la section et la stocke dans la structure SectionTable
 Affiche la taille suivant le paramètre verbose.
 La taille est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur SectionTable de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords: Modifie la structure SectionTable, ajoute la taille à sh_entsize
 */
void sectionEntrySize(FILE *file, Elf32_Shdr* SectionTable, char verbose);

/* Affiche le numéro de la section entre crochets aligné à droite
 Par exemple pour 101 sections à afficher: [  0] ... [ 10] ... [100]

 Paramètre: un pointeur Header de structure d'en-tête, un entier correspondant au numéro de la section à afficher.
 Sortie : Ne renvoie rien
 Effets de bords : Affiche le numéro de la section, entre crochets, aligné à droite*/
void printNumber(Elf32_Ehdr* Header, int sectionNumber);


/* Remplit la structure SectionTable avec une fonction pour chaque membre

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, 
			un pointeur SectionTable de structure d'en-tête de section, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure SectionTable, la remplie entièrement
 */
void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);
#endif /* readSectionTable_h */
