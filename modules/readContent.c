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
	uint32_t dumped=0;
	char buffer[8];

	while (!end){

		if ((SectionTable->sh_size - dumped) < 8){
			/* End in sight */
			sizeToRead=SectionTable->sh_size - dumped;
			//printf("Size to read : %d", sizeToRead);
		}
		if (dumped%16 == 0){
			/* On revient à la ligne tous les 16 octets */
			printf("\n");
			printf("0x%08x ",adressPrinted);
		}

		fread(buffer, sizeToRead, 1, file);
		dumpOctet(buffer, sizeToRead);
		dumped+=sizeToRead;
		adressPrinted+=8;

		if (dumped == SectionTable->sh_size){
			/* On a affiché toutes les données */
			end=1;
			printf("\n");
		}
	}
}
