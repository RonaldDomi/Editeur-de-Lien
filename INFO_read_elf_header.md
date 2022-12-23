On cherche à lire le header (en-tête) d'un .elf, le header est défini dans la _Spécification du Format ELF_ page 4
On peut lire des fichiers binaires en C, on utilise l'option 'b' lors du fopen() pour spécifier un fichier binaire

##Lire un fichier binaire en cmd:

On peut utiliser la commande hexdump fichier pour lire au format binaire, le résultat sera au format hexadécimal:

Exemple: hexdump -C tests/firmware.elf | head :
adr     data
0000000 457f 464c 0101 0001 0000 0000 0000 0000 Magic Number
0000010 0002 0028 0001 0000 0bd5 0800 0034 0000 ObjectType, MachineType, 
0000020 d058 0003 0200 0500 0034 0020 0002 0028
.....
On note la convention Big Endiant, il faut donc lire 7F 45 4C 46 ....

##Lire avec readelf -h:

Le but de cette étape est d'implémenter la même fonctionnalité que la commande readelf -h <FichierBinaire>
On peut donc beaucoup s'aider de cette commande pour l'affichage.
De plus, on considérera cette commande comme un oracle lors de la vérification automatique.

##Lire avec read_elf_header :
En même temps que la lecture, on remplit une structure ELF_Header qui est la même que celle spécifiée dans elf.h.

La lecture et l'affichage se fait avec le module freadoctet disponible dans le dossier modules.
Le remplissage de la structure se fait avec le module readHeader disponible dans le dossier modules

Le header commence par le magic number composé de 16 octets:
	On l'affiche simplement pas besoin de le décoder.

On continue ensuite pour tous les autres membres en faisaint attention au type:
	Les informations codées sur un Word demandent de lire 4 octets.
	Les informations codées sur un Half(-Word) demandent de lire 2 octets.
	Les offsets sont équivalents aux Word et demandent donc 4 octets.
	Les adresses sont équivalents aux Word et demandent aussi 4 octets.

On lit le fichier de façon linéaire en incrémentant le pointeur de lecture à chaque read.
Seule exception est après de la vérification du Magic Number où on retourne au début de fichier.



## Générer des fichiers .elf pour des tests :
https://clang.llvm.org/docs/CrossCompilation.html

On peut générer des .elf pour différentes machines en utilisant la cross-compilation de clang (ou gcc équivalent) :

Par exemple pour générer un elf pour un ARM V7:
clang --target=arm-v7-pc-linux-gnu -mfloat-abi=soft -c main.c

Ou pour un i386:
clang --target=i386-pc-linux-gnu -c main.c

Ne pas générer des tests pour un processeur 64 bits, les ELF 64 bits sont en dehors du cadre du projet.
