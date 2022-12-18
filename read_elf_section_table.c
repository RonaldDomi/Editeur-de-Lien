//
//  read_elf_section_table.c
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 18/12/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_elf_section_table.h"
#include "modules/CustomElf.h"
#include "modules/freadoctet.h"

int checkELF(FILE *file){
	char buffer[4];
	fread(buffer, 4, 1, file);
	fseek(file, -4, SEEK_CUR); /* Go backwards 4 bytes */
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
	return 1;
}


int main(int argc, char **argv){

	if (argc<2){
		printf("Usage : ./read_elf_header <FichierBinaire>\n");
		exit(1);
	}

	FILE *file = fopen(argv[1],"rb");
	if (file==NULL){
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	if (!checkELF(file)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	/* On cherche à aller directement a la section table, pour cela on a besoin du Offset situé dans le header */
	/* 1: On peut soit utiliser l'étape 1, lire tout le header, et récupérer l'Offset de la structure Header */
	/* OU */
	/* 2: On saute directement à l'Offset, on le lit, on va à la Section Table */

	/* Méthode 2: */
	/* On sait que l'offset est à 28 bytes du début (on peut vérifier en faisant la somme des types de la structure du Header jusqu'à l'Offset */
	/* On saute donc directment de 28 bytes */
	fseek(file, 28, SEEK_CUR);

	/* On lit l'offset */
	uint32_t buffer;
	fread(&buffer, 4, 1, file);

	/* On saute de buffer-28 comme l'offset part du début de fichier */
	fseek(file, buffer-28, SEEK_CUR)


	return 0;
}
