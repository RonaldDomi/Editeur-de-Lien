#include <stdio.h>
#include <stdlib.h>
#include "dependencies/freadoctet.h"
//#include "elf.h"

void magicNumber(FILE *file){
	printf("Magic number : ");
    char * buffer = readOctet(file, 16);
	printOctet(buffer, 16);
	printf("\n");
}

// void objectType(FILE *file){
// 	char buffer[1];

// 	printf("Type objet : ");

// 	printBinary(buffer, 1);
// 	switch(buffer[0]){
// 		case(0):
// 			printf(": No type");
// 			break;
// 		case(1):
// 			printf(": Relocatable file");
// 			break;
// 		case(2):
// 			printf(": Executable file");
// 			break;
// 		case(3):
// 			printf(": Shared object file");
// 			break;
// 		case(4):
// 			printf("Core file");
// 			break;
// 		default:
// 			printf("===WARNING: Unable to determine object type !===");
// 			break;
// 	}
// 	printf("\n");

// }
int main(int argc, char **argv){

	if (argc<2){
		printf("Usage : ./ReadBitaBit <FichierBinaire>\n");
		exit(1);
	}

    FILE *file = fopen(argv[1],"rb");
    if (file==NULL){
        printf("Erreur lors de l'ouverture du fichier!\n");
        exit(1);
    }
	printf("ELF Header Reader : \n\n");
	magicNumber(file);
	// objectType(file);
	fclose(file);
}
