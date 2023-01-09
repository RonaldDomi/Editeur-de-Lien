//
//  CheckElf.h
//  Editeur de Liens
//

#include <stdio.h>
#include <stdlib.h>

/*
 Vérifie si le fichier est de type ELF
 On vérifie les 4 premiers nombres magiques en début de fichier qui doivent correspondrent à la séquence suivante :
 7F 45 4C 46 qui se translate par "7F ELF"
 Si la vérification est correcte, on replace le pointeur en début de fichier.
 Sinon, une exception sera levée par un autre programme donc le déplacement n'est pas nécéssaire.

 Paramètre: Pointeur 'file' vers un fichier
 Sortie: 1 si le fichier est de type ELF, 0 sinon
 */
int checkELF(FILE *file);
