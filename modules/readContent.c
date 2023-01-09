//
//  readContent.c
//  Editeur de Liens
//

#include <stdio.h>
#include "CustomElf.h"
#include "freadoctet.h"
#include "readHeader.h"
#include "readSectionTable.h"
#include "readContent.h"


int unused; // Var non utilisée pour les warnings lors du make

void getAllSectionsContent(FILE* file, Elf32_AllSec * SectionsTables){
    Elf32_Shdr * currentSectionTable;
    char * currentSectionContent;
    
    for (int i=0; i<SectionsTables->nbSections; i++){
        currentSectionTable = SectionsTables->TabAllSec[i];
        currentSectionContent = malloc(currentSectionTable->sh_size);
        fseek(file, currentSectionTable->sh_offset, SEEK_SET);
        unused = fread(currentSectionContent, currentSectionTable->sh_size, 1, file);
        SectionsTables->TabAllSecContent[i] = currentSectionContent;
    }
}

void printContent(Elf32_AllSec *SectionsTables, int sectionSelected)
{
	printf("\nDump of section :\n");

	int adressPrinted = SectionsTables->TabAllSec[sectionSelected]->sh_addr;
    printf("\n0x%08x ", adressPrinted);
    adressPrinted += 16;

    for(int i = 1;i<=SectionsTables->TabAllSec[sectionSelected]->sh_size;i++){
        printf("%02x", (unsigned char)SectionsTables->TabAllSecContent[sectionSelected][i-1]);
        if (i%16==0 && SectionsTables->TabAllSec[sectionSelected]->sh_size != i){
            printf("\n0x%08x ", adressPrinted);
            adressPrinted += 16;
        }
        else if (i%4==0) 
            printf(" ");
    }
    printf("\n");


}
