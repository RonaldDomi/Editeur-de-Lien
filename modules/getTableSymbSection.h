//
//  getTableSymbSection.h
//  Editeur de Liens
//


#ifndef getTableSymbSection_h
#define getTableSymbSection_h


/* Décode les octets de shndx de la table des symboles en une chaîne de caractères compréhensible.

 Paramètres: Un pointeur vers une table de symboles

 Valeur de retour: Ne retourne rien
 Effets de bords : Ecrit le shndx décodé sur la sortie standard
 */
void decodeSymbShndx(Elf32_Sym * symtab);


/* Décode les octets de type et d'informations de la table des symboles en une chaîne de caractères compréhensible.

 Paramètres: Un pointeur vers une table de symboles

 Valeur de retour: Ne retourne rien
 Effets de bords: Ecrit le type et l'information décodés sur la sortie standard
 */
void decodeSymbType(Elf32_Sym * symtab);


/* Décode les octets de other de la table des symboles en une chaîne de caractères compréhensible.

 Paramètres: Un pointeur vers une table de symboles

 Valeur de retour: Ne retourne rien
 Effets de bords: Ecrit le other décodé sur la sortie standard
 */
void decodeSymbOther(Elf32_Sym * symtab);


/* Décode l'entièreté de la table des symboles en une chaîne de caractères compréhensible.

 Paramètres: Un pointeur symtab vers une table de symboles
			Un pointeur Sections vers la table des sections
			Un pointeur Header vers l'en-tête

 Valeur de retour: Ne retourne rien
 Effets de bords : Ecrit toutes les informations décodées sur la sortie standard
 */
void printTableSymb(Elf32_Sym * symtab, Elf32_AllSec * Sections, Elf32_Ehdr * Header);


/* Décode l'entièreté de la table des symboles en une chaîne de caractères compréhensible.

 Paramètres: Un pointeur AllSymbolTables vers les table de symboles
 Un pointeur AllSectionsTables vers les table des sections
 Un pointeur Header vers l'en-tête

 Valeur de retour: Ne retourne rien
 Effets de bords : Ecrit toutes les informations décodées sur la sortie standard
 */
void printAllTableSymb(Elf32_Sym * AllSymbolTables, Elf32_AllSec * AllSectionsTables, Elf32_Ehdr * Header);


/* Recupère les informations et remplit une table de symboles

 Paramètres: Un pointeur file vers un fichier
			Un pointeur Header vers l'en-tête
			Un pointeur SectionContent vers le contenu d'une section
			Un pointeur symtab vers une table de symboles
	TODO: Enlever si sscanf inutilisé Un entier adrligne indiquant la ligne à lire

 Valeur de retour: Ne retourne rien
 Effets de bords : Ecrit toutes les informations décodées sur la sortie standard
 */
void GetTableSymbPart(FILE *file, Elf32_Ehdr *Header, Elf32_SecContent SectionContent, Elf32_Sym *symtab, int adrligne);


/* Recupère toutes les informations les informations et remplit toutes les tables de symboles

 Paramètres: Un pointeur file vers un fichier
 Un pointeur Header vers l'en-tête
 Un pointeur SectionContent vers le contenu d'une section
 Un pointeur symtab vers une table de symboles
 Un entier nbTable indiquant la le nombre de tables à remplir

 Valeur de retour: Ne retourne rien
 Effets de bords : Ecrit toutes les informations décodées sur la sortie standard
 */
void getAllTableSymb(FILE *file, Elf32_Ehdr *Header, Elf32_SecContent SectionContent, Elf32_Sym *AllSymbolTables, int nbTable);
#endif /* getTableSymbSection_h */
