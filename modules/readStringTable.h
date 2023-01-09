//
//  readStringTable.h
//  Editeur de Liens
//

#ifndef readStringTable_h
#define readStringTable_h


/* Alloue 50 chars et recupère le nom de la section
	Ne libère *pas* la mémoire

Paramètres:Un entier index sur la section à récupérer,
			Un pointeur vers une en-tête d'ELF, un pointeur vers toutes les sections d'un ELF
 Valeur de retour: Retourne un pointeur vers le nom
 Effets de bords : Alloue de la mémoire pour le nom
*/
char * getStringSection(Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections);

/* Alloue 50 chars et recupère le nom du symbole
	Ne libère *pas* la mémoire

 Paramètres:Un entier index sur la section à récupérer,
			Un pointeur vers une en-tête d'ELF, un pointeur vers toutes les sections d'un ELF
 Valeur de retour: Retourne un pointeur vers le nom
 Effets de bords : Alloue de la mémoire pour le nom
 */
char * getStringSymbol(Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections);

#endif /* readStringTable_h */
