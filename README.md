# test git push zeynel
# Editeur-de-Lien
Editeur de Lien implemente en C

Les fichiers ELF sont des fichiers objets qui peuvent être:
• Un executable
• Un relocatable object (??)
• Un objet partagé par d'autres fichiers objets

Ces fichiers sont donc au format *binaire*, on cherche donc à lire bit-à-bit
## Phase 1 : Etape 1 :

On cherche à lire le header (en-tête) d'un .elf, le header est défini dans la _Spécification du Format ELF_ page 4
On peut lire des fichiers binaires en C, on utilise l'option 'b' lors du fopen() pour spécifier un fichier binaire
