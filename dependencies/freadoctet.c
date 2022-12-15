#include <stdio.h>
#include <stdlib.h>

void printOctet(char buffer[], int length){
	for(int i = 0; i<length; i++)
		printf("%x ", buffer[i]); // prints a series of bytes
}

char * readOctet(FILE * file, int nbOctet){
    char * buffer = malloc(sizeof(char)*nbOctet);
    fread(buffer, sizeof(buffer), 1, file);
    return buffer;
}