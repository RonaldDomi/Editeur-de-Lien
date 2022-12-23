#include <stdio.h>
#include <stdlib.h>

int checkELF(FILE *file){
	char buffer[4];
	fread(buffer, 4, 1, file);
	/* On vérifie si les 4 premiers nombres sont bien 7F 45 4C 46
	qui correspondent au Magic Number d'un ELF */
	if (feof(file)){
		return 0;
	}
	if (buffer[0] != 0x7F){
		return 0;
	}
	if (buffer[1] != 0x45){
		return 0;
	}
	if (buffer[2] != 0x4C){
		return 0;
	}
	if (buffer[3] != 0x46){
		return 0;
	}
	fseek(file, 0, SEEK_SET);
	return 1;
}
