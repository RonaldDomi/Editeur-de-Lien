//
//  readTableSymbole.c
//  Editeur de Liens
//
#include <stdio.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"

void GetRelocationPart(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
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
            if(SectionTable->sh_type == SHT_REL) return;
		}
	}
}

void offset(FILE* file, Elf32_Rel* rel, char verbose)
{
    fread(&rel->r_offset, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_offset, 4, 1);
        printf("\t");
    }
}

void info(FILE* file, Elf32_Rel* rel, char verbose)
{
    fread(&rel->r_info, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_info+1,4,1);
        printf("\t");
        printOctet(&rel->r_info,4,1);
        printf("\t");
    }
}

// void addend(FILE* file, Elf32_Rel* rel, char verbose)
// {
//     fread(&rel->r_offset, 4, 1, file);
//     if(verbose)
//     {
//         printOctet(&rel->r_offset, 4, 1);
//         printf("\t\t");
//     }
// }

void get_relocation(FILE* file, Elf32_Rel* rel, char verbose)
{
    offset(file, rel, verbose);
    info(file, rel, verbose);
    // addend(file, rel, verbose);
}
