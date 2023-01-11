# Documentation Partie 1 : 

## Affichage d'un fichier objet/binaire:
<hr>
On peut utiliser la commande hexdump fichier pour lire au format binaire, le résultat sera au format hexadécimal :

Exemple: `hexdump -C tests/firmware.elf | head :`
  
>adr     data
>0000000 7f45 4c46 0101 0100  0000 0000 0000 0000 Magic Number  
>0000010 0200 2800 0100 0000  d50b 0008 3400 0000 ObjectType, MachineType,   
>0000020 58d0 0300 0002 0005  3400 2000 0200 2800
>.....
     
Tous les fichiers testés sont pour un processeur ARM 32 Bits *Little Endian*
<br>
<br>
## Processus de lire/traiter les fichiers objets :  
<hr>
Pour traiter un fichier objet, on lit le fichier de façon linéaire en incrémentant le pointeur de lecture à chaque read.   
Pendant le read, on complète la structure ELF en faisant aux tailles de structures : 

• Word demandent de lire 4 octets.  
• Half(-Word) demandent de lire 2 octets.  
• Offsets demandent de lire 4 octets.  
• Adresses demandent de lire  4 octets.  

<br>

## Générer des fichiers ELF pour des tests :
<hr>
https://clang.llvm.org/docs/CrossCompilation.html

On peut générer des .elf pour différentes machines en utilisant la cross-compilation de clang (ou gcc équivalent) :
On doit déjà installer le cross-compiler arm-none-eabi-gcc

Ensuite, pour générer un fichier objet ARM V5 32Bits :
`arm-none-eabi-gcc -c sources/simple.c -mbe32`

Ne pas générer des tests pour un processeur 64 bits, les ELF 64 bits sont en dehors du cadre du projet. Ne pas générer des tests pour un type de processeur autre que ARM qui est aussi hors-projet.

