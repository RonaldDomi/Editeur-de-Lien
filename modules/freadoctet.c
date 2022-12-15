#include <stdio.h>
#include <stdlib.h>

void printOctet(void * buffer, int length, char reverse){
	if (reverse){
		for(int i = length - 1; i>=0; i--)
			printf("%02x ", ((char *)buffer)[i]); // prints a series of bytes
	}else{
		for(int i = 0; i<length; i++){
			printf("%02x ", ((char *)buffer)[i]); // prints a series of bytes
		}
	}
}

void * readOctet(FILE * file, int nbOctet){
	void * buffer = malloc(sizeof(char)*nbOctet);
    fread(buffer, sizeof(char)*nbOctet, 1, file);
    return buffer;
}
