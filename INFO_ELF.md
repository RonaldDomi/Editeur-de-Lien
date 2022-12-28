## 



## Affichage d'un fichier objet/binaire:
<hr>
On peut utiliser la commande hexdump fichier pour lire au format binaire, le résultat sera au format hexadécimal:

Exemple: hexdump -C tests/firmware.elf | head :  
adr     data
0000000 457f 464c 0101 0001 0000 0000 0000 0000 Magic Number  
0000010 0002 0028 0001 0000 0bd5 0800 0034 0000 ObjectType, MachineType,   
0000020 d058 0003 0200 0500 0034 0020 0002 0028  
.....  
On note la convention Big Endiant, il faut donc lire 7F 45 4C 46 ....   
Tout les fichiers qu'on utilise comme testers vont etre pour un processeur 32 bits
<br>
<br>
## Procces de lire/traiter les fichiers objet :  
<hr>
Pour traiter le fichier objet, on lit le fichier de façon linéaire en incrémentant le pointeur de lecture à chaque read.   
Pendant la read, on complete la structure ELF faisant attention a ce qui est attendue: 

Word demandent de lire 4 octets.  
Half(-Word) demandent de lire 2 octets.  
Offsets demandent de lire 4 octets.  
Adresses demandent de lire  4 octets.  

Seule exception est après de la vérification du Magic Number où on retourne au début de fichier.
<br>
<br>
## Générer des fichiers .elf pour des tests :
<hr>
https://clang.llvm.org/docs/CrossCompilation.html

On peut générer des .elf pour différentes machines en utilisant la cross-compilation de clang (ou gcc équivalent) :

Par exemple pour générer un elf pour un ARM V7:
clang --target=arm-v7-pc-linux-gnu -mfloat-abi=soft -c main.c

Ou pour un i386:
clang --target=i386-pc-linux-gnu -c main.c

Ne pas générer des tests pour un processeur 64 bits, les ELF 64 bits sont en dehors du cadre du projet.
