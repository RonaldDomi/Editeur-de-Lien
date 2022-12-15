Les fichiers ELF sont des fichiers objets qui peuvent être:
• Un executable
• Un relocatable object (??)
• Un objet partagé par d'autres fichiers objets

Ces fichiers sont donc au format *binaire*, on cherche donc à lire bit-à-bit
## Phase 1 : Etape 1 :

On cherche à lire le header (en-tête) d'un .elf, le header est défini dans la _Spécification du Format ELF_ page 4
On peut lire des fichiers binaires en C, on utilise l'option 'b' lors du fopen() pour spécifier un fichier binaire

##Lire un fichier binaire en cmd:

On peut utiliser la commande hexdump fichier pour lire au format binaire, le résultat sera au format décimal:

Exemple: hexdump -C tests/firmware.elf | head :
adr     data
0000000 457f 464c 0101 0001 0000 0000 0000 0000
0000010 0002 0028 0001 0000 0bd5 0800 0034 0000
0000020 d058 0003 0200 0500 0034 0020 0002 0028
.....

On note la convention Big Endiant, il faut donc lire 7F 45 4C 46 ....
7f 45 4c 46 1 1 1 0 0 0 0 0 0 0 0 0 2 0 28 0 1 0 0 

Le header commence par le magic number composé de 16 octets:
	On l'affiche simplement pas besoin de le décoder
	
Ensuite on dispose du type de fichier objet, ce que l'on doit décoder:
Tiré de elf.h
</* Legal values for e_type (object file type).  */>
<#define ET_NONE	0		/* No file type */>
<#define ET_REL		1		/* Relocatable file */>
<#define ET_EXEC	2		/* Executable file */>
<#define ET_DYN		3		/* Shared object file */>
<#define ET_CORE	4		/* Core file */>
<#define ET_NUM		5		/* Number of defined types */>
<#define ET_LOOS	0xfe00		/* OS-specific range start */>
<#define ET_HIOS	0xfeff		/* OS-specific range end */>
<#define ET_LOPROC	0xff00		/* Processor-specific range start */>
<#define ET_HIPROC	0xffff		/* Processor-specific range end */>

