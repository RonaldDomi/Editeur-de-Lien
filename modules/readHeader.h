//
//  readHeader.h
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 20/12/2022.
//

#ifndef readHeader_h
#define readHeader_h

#include <stdio.h>
#include "CustomElf.h"

/* Lit le nombre magique d'un fichier ELF et le stocke dans la structure Header
	Affiche le nombre suivant le paramètre verbose.
	Le nombre magique est sur 16 octets, on lit donc 16 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute le magic number à e_indent
 */
void magicNumber(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit le type objet du fichier et le stocke dans la structure Header.
	Affiche le type et le détail suivant le paramètre verbose.
	Le type est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute le numéro de type à e_type (et pas le détail)
 */
void objectType(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit l'architecture de l'ELF et la stocke dans la structure Header.
 Affiche l'architecture et le détail suivant le paramètre verbose.
 L'architecture est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute le numéro de type à e_machine (et pas le détail)
 */
void machineType(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit la version de l'ELF et la stocke dans la structure Header.
 Affiche la version et le détail suivant le paramètre verbose.
 La version est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute le numéro de type à e_version (et pas le détail)
 */
void version(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit l'adresse d'entrée et la stocke dans la structure Header.
 Affiche l'adresse suivant le paramètre verbose.
 L'adresse est un Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute l'adresse à e_entry
 */
void entry(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit le décalage au début de programme et la stocke dans la structure Header.
 Affiche le décalage suivant le paramètre verbose.
 Le décalage est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_phoff
 */
void progHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit le décalage au début de section et la stocke dans la structure Header.
 Affiche le décalage suivant le paramètre verbose.
 Le décalage est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_shoff
 */
void sectHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose);


/* Lit les flags prévus et les stockent dans la structure Header.
 === ATTENTION: Le détail n'est pas (encore) disponible pour cette caractéristique, bien qu'il serait pratique...
				Pour l'instant on affiche donc que la valeur qui n'est pas parlante par elle-même ===
 Affiche le décalage suivant le paramètre verbose.
 La valeur est un Word, on lit donc 4 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_flags (Les détails ne seront pas ajoutés)
 */
void flags(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Lit la taille de l'en-tête et la stocke dans la structure Header.
 Affiche la taille suivant le paramètre verbose.
 La taille est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_ehsize
 */
void headerSize(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Lit la taille de l'en-tête de chaque programme et la stocke dans la structure Header.
 Affiche la taille suivant le paramètre verbose.
 Le nombre est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_phentsize
 */
void headerProgramSize(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Lit le nombre d'en-têtes de programmes et le stocke dans la structure Header.
 Affiche la taille suivant le paramètre verbose.
 Le nombre est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_phnum
 */
void nbProgramHeaders(FILE *file,Elf32_Ehdr *Header, char verbose);


/* Lit la taille de la section de chaque programme et la stocke dans la structure Header.
 Affiche la taille suivant le paramètre verbose.
 La taille est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute la valeur à e_shentsize
 */
void sizeSectionHeaders(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Lit le nombre d'en-tête et le stocke dans la structure Header.
 Affiche le nombre suivant le paramètre verbose.
 La taille est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute le nombre à e_shnum
 */
void nbSectionHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Lit l'index de la String Table et le stocke dans la structure Header.
 Affiche l'index suivant le paramètre verbose.
 Le nombre est un Half-Word, on lit donc 2 octets.

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, ajoute l'index à e_shstrndx
 */
void indexStringHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


/* Remplit la structure Header avec une fonction pour chaque membre

 Paramètre: un pointeur file de fichier, un pointeur Header de structure d'en-tête, un booléen verbose pour gérer l'affichage
 Sortie : Ne renvoie rien
 Effets de bords : Modifie la structure Header, la remplie entièrement
 */
void getHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


#endif /* readHeader_h */
