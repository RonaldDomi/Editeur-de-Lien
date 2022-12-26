//
//  readContent.c
//  Editeur-de-lien
//
#include "readHeader.h"
#include "readSectionTable.h"
#include "readContent.h"
#include "CustomElf.h"
#include "freadoctet.h"

void printContent(FILE* file, Elf32_Shdr* SectionTable, int sectionSelected, char* nameOfSection){
	int adress = SectionTable->sh_addr + SectionTable->sh_offset + SectionTable->sh_addr;

	if (sectionSelected==-1){
		printf("\nDump of section %s :\n", nameOfSection);
	}else{
		printf("\nDump of section number %d :\n", sectionSelected);
	}

	printf("Starting at 0x%08x \n", adress);

	fseek(file, adress, SEEK_SET);

	int end=0;
	int adressPrinted=0;
	int sizeToRead=8;
	long dumped=0;
	char buffer[8];

	while (!end){

		if ((8 - SectionTable->sh_size) < 0){
			printf("End in sight");
			sizeToRead=(8 - SectionTable->sh_size);
		}
		if (dumped%16 == 0){
			printf("\n");
			printf("0x%08x ",adressPrinted);
		}

		fread(buffer, sizeToRead, 1, file);
		dumpOctet(buffer, sizeToRead);
		dumped+=8;
		adressPrinted+=8;

		if (dumped == SectionTable->sh_size){
			end=1;
			printf("\n");
		}
	}
}
