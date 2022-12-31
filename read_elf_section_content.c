//
//  read_elf_section_content.c
//  Editeur-de-lien
//
//  Created by Bastien Levasseur on 26/12/2022.
//

#include <stdio.h>
#include <stdlib.h>

#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/readContent.h"
#include "modules/freadoctet.h"

int main(int argc, char **argv){

	/* Vérifie le nombre d'arguments */
	if (argc<3){
		printf("Usage : ./read_elf_section_content <File> <SectionNumber>\n");
		exit(1);
	}

	/* Tente d'ouvrir un fichier avec son nom donné en argument */
	FILE *file = fopen(argv[1],"rb");
	if (file==NULL){
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	/* Vérifie que le fichier est un ELF, renvoie une exception sinon*/
	if (!checkELF(file)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	/* Alloue de la mémoire pour une en-tête et la remplit
	 Alloue aussi de la mémoire pour une en-tête de section */

	Elf32_Ehdr* Header = malloc(sizeof(Elf32_Ehdr));

	getHeader(file, Header, 0);
	fseek(file, Header->e_shoff, SEEK_SET);

	Elf32_Shdr* SectionTable = malloc(sizeof(Elf32_Shdr));

	char* endPointer = NULL;
	int sectionSelected = strtol(argv[2], &endPointer, 10);

	if (endPointer==argv[2]){

		/* Search by Name */
		printf("Search by Name\n");
		if (getSectionName(file, Header, SectionTable, argv[2], 0)){
			printf("Section Found!\n");
			printContent(file, SectionTable, -1, argv[2]);
		}else{
			printf("\nThere are no section called \"%s\".", argv[2]);
			exit(1);
		}

	}else{

		/* Search by Number */
		printf("Search by Number\n");
		if (sectionSelected > Header->e_shnum){
			printf("This section does not exist !\n");
			printf("There are only %d sections.\n", Header->e_shnum);
			exit(1);
		}
		getSectionTable(file, Header, SectionTable, sectionSelected, 0);

		if (SectionTable->sh_size == 0){
			printf("There is no data to dump.\n");
			exit(0);
		}
		printContent(file, SectionTable, sectionSelected, NULL);
	}

	return 0;
}

