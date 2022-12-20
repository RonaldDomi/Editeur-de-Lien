//
//  read_elf_section_table.c
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 18/12/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "modules/readHeader.h"
#include "modules/freadoctet.h"
#include "modules/CheckElf.h"

void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		printf("Nb\tName\tType\tFlags\tExecution adresss\tOffset\tSection Size\tLink To\tInfo\tAlign\tEntry Size");
	}
	//TODO: Faire toutes les fonctions
}


int main(int argc, char **argv){

	if (argc<2){
		printf("Usage : ./read_elf_section_table <FichierBinaire>\n");
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
	
	Elf32_Ehdr* Header = malloc(sizeof(Elf32_Ehdr));
	Elf32_Shdr* SectionTable = malloc(sizeof(Elf32_Shdr));

	getHeader(file, Header, 0);

	getSectionTable(file, Header, SectionTable, 1);

	return 0;
}
