#include <stdio.h>
#include <stdlib.h>

/* Affiche octet par octet au format décimal avec deux chiffres pour chaque octet
 Suivant le paramètre reverse, commence par le début ou la fin du tableau
 Cela permet de prendre en compte le Big Endian où non.

 Paramètres: Un pointeur vers une valeur, la longueur length de la valeur, un booléen reverse
 Sortie: Ne renvoie rien
 Effets de bords: Affiche octet par octet au format hexadécimal
 */
void printOctet(void *buffer, int length, char reverse);

/* Affiche une adresse au format héxadécimal avec le préfixe 0x
 On prend le même format que readelf, donc les adresses sont affichées sur 7 bits
 Suivant le paramètre reverse, commence par le début ou la fin du tableau
 Cela permet de prendre en compte le Big Endian où non.

 Paramètres: Un pointeur vers une valeur, la longueur length de la valeur, un booléen reverse
 Sortie: Ne renvoie rien
 Effets de bords: Affiche octet par octet au format hexadécimal
 */
void printAdress(void *buffer, int length, char reverse);

/* Affiche une adresse au format héxadécimal avec le préfixe 0x
 On prend le même format que readelf, donc les adresses sont affichées sur 8 bits
 Suivant le paramètre reverse, commence par le début ou la fin du tableau
 Cela permet de prendre en compte le Big Endian où non.

 Paramètres: Un pointeur vers une valeur, la longueur length de la valeur, un booléen reverse
 Sortie: Ne renvoie rien
 Effets de bords: Affiche octet par octet au format hexadécimal
 */
void printAdress8(void *buffer, int length, char reverse);

/* Affiche des données au format hexadécimal, par paquets de 4 octets
 On prend le même format que readelf -x donc on affiche des paquets de 4 octets séparés par un espace

 Paramètres: Un pointeur vers une valeur, la longueur length de la valeur
 Effets de bords: Affiche octet par octet par paquet de 4 au format hexadécimal
 */
void dumpOctet(void *buffer, int length);

/* Alloue de l'espace pour nbOctets, lit nbOctets sur le fichier et les stockent dans l'espace alloué

 Paramètres: Un pointeur file vers un fichier, le nombre d'octets à lire
 Sortie: Renvoie un pointeur vers la mémoire allouée contenant le contenu lu
 */
void *readOctet(FILE *file, int nbOctet);
