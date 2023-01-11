Ce répertoire contient des tests **Little Endian** qui permettent de tester les programmes:
	• Firmware.elf est un fichier ELF provenant d'une carte STM32F4 étudié dans l'UE ALM.
	• notELF.elf est un fichier binaire contenant un seul bit (il n'est donc pas un fichier ELF).
	• memeFonc1.o & memeFonc2.o sont deux fichiers contenant la même fonction memeFonction(), memeFonc2 appelle memeFonc1.
	• sansInclude.o est un fichier sans aucune inclusion.
	• salut1.o & salut2.o sont deux fichiers où salut2 dépend de salut1.
	• simple.c est un fichier contenant juste un printf().
	
Toutes les sources des tests excepté firmware.elf sont disponibles sous le répertoire sources/.
