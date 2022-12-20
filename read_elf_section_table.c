//
//  read_elf_section_table.c
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 18/12/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"

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

	fseek(file, Header->e_shoff, SEEK_SET);
	printf("\n");
	printf("Adresses are given un hexadecimal format.\n");
	printf("All values are given in bytes in decimal format.\n\n");
	printf("Nb\tName\t\tType\t\tFlags\tExecutionAdresss\tOffset\t\tSectionSize\tLinkTo\tInfo\tAlign\tEntrySize\n");
	printf("====================================================================");
	printf("=====================================================================\n");
	
	getSectionTable(file, Header, SectionTable, 1);

	return 0;
}
