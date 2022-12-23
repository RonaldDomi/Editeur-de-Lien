//
//  read_elf_table_symbol.c
//  Editeur de Liens
//
#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/getTableSymbSection.c"

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
	printf("Nb\tName\t\tValue\t\tSize\tBind\tType\t\tOther\tShndx\n");
	printf("====================================================================");
	printf("=====================================================================\n");
	
	GetTableSymbPart(file, Header, SectionTable, 0);
	//on recupere le decalage de la table symbole puis on se decale
	fseek(file, SectionTable->sh_offset, SEEK_SET);	

	Elf32_Sym* symTab = malloc(sizeof(Elf32_Sym));
	for (int i = 0; i < SectionTable->sh_size/16; i++)
	{
		printf("%d\t", i);
		getTabSymb(file, symTab, 1);
		printf("\n");
	}
	

	return 0;
}
