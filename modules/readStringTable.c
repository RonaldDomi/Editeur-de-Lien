
#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"

void GetTableStringPart(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
	if (Header->e_shnum == 0){
		if (verbose){
			printf("No section Table...\n");
		}
	}else{
		for (int i=0; i<Header->e_shnum; i++){
			if (verbose){
				printf("[%d]\t", i);
			}
			sectionName(file, Header, SectionTable, verbose);
			sectionType(file, SectionTable, verbose);
			sectionFlags(file, SectionTable, verbose);
			sectionAdress(file, SectionTable, verbose);
			sectionOffset(file, SectionTable, verbose);
			sectionSize(file, SectionTable, verbose);
			sectionLink(file, SectionTable, verbose);
			sectionInfo(file, SectionTable, verbose);
			sectionAdressAlign(file, SectionTable, verbose);
			sectionEntrySize(file, SectionTable, verbose);
			
			if (verbose){
				printf("\n");
			}
            if(SectionTable->sh_type == SHT_STRTAB) return;
		}
	}
}

void getString(FILE* file, Elf32_Word index, Elf32_Ehdr* Header, char* mot){
    Elf32_Shdr* strTabSect = malloc(sizeof(Elf32_Shdr));
    fseek(file, Header->e_shoff, SEEK_SET);
    GetTableStringPart(file, Header, strTabSect, 0);
    fseek(file, strTabSect->sh_offset, SEEK_SET);
    fseek(file, index, SEEK_CUR);
    int i = 0;
    while (mot[i] = fgetc(file))
    {
        i++;
    }
}