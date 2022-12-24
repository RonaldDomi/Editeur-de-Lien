#include <stdio.h>
#include <stdlib.h>

void printOctet(void * buffer, int length, char reverse){
	/* On met le type unsigned char pour éviter un comportement étrange
	 Avec char, le compilateur comprend un type signé, qui peut mener à l'ajout de plusieurs F à l'affichage
	 Le problème est décrit ici : https://stackoverflow.com/a/31090695/14702675 */

	if (reverse){ //On commence par buffer[3], buffer[2]...
		for(int i = length - 1; i>=0; i--){
			printf("%02x ", ((unsigned char *)buffer)[i]);
		}
	}else{ //On commence par buffer[0], buffer[1]...
		for(int i = 0; i<length; i++){
			printf("%02x ", ((unsigned char *)buffer)[i]);
		}
	}
}


void printAdress(void * buffer, int length, char reverse){
	/* Même remarque pour le type unsigned char */
	printf("0x");
	if (reverse){ //On commence par buffer[3], buffer[2]...
		printf("%01x", ((unsigned char *)buffer)[length - 1]);
		for(int i = length - 2; i>=0; i--){
			printf("%02x", ((unsigned char *)buffer)[i]);
		}
	}else{ //On commence par buffer[0], buffer[1]...
		printf("%01x", ((unsigned char *)buffer)[0]);
		for(int i = 1; i<length; i++){
			printf("%02x", ((unsigned char *)buffer)[i]);
		}
	}
}


void printAdress8(void * buffer, int length, char reverse){
	/* Même remarque pour le type unsigned char */
	printf("0x");
	if (reverse){ //On commence par buffer[3], buffer[2]...
		printf("%02x", ((unsigned char *)buffer)[length - 1]);
		for(int i = length - 2; i>=0; i--){
			printf("%02x", ((unsigned char *)buffer)[i]);
		}
	}else{ //On commence par buffer[0], buffer[1]...
		printf("%02x", ((unsigned char *)buffer)[0]);
		for(int i = 1; i<length; i++){
			printf("%02x", ((unsigned char *)buffer)[i]);
		}
	}
}


void * readOctet(FILE * file, int nbOctet){
	void * buffer = malloc(sizeof(char)*nbOctet);
    fread(buffer, sizeof(char)*nbOctet, 1, file);
    return buffer;
}
