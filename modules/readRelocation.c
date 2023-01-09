//
//  readRelocation.c
//  Editeur de Liens
//

#include <stdio.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"
#include "readStringTable.h"

int unused; // Var non utilisée pour les warnings lors du make

void GetRelocation_rel(FILE *file, Elf32_Rel *Rel){
	unused = fread(&Rel->r_offset, 4, 1, file);

    printOctet(&Rel->r_offset, 4, 1);
	printf("\t");

    unused = fread(&Rel->r_info, 4, 1, file);

    printOctet(&Rel->r_info + 1, 4, 1);
    printf("\t");
    printOctet(&Rel->r_info, 4, 1);
    printf("\t%d\t\t", (char)Rel->r_info);
}


void GetRelocation_rela(FILE *file, Elf32_Rela *Rela){
	unused = fread(&Rela->r_offset, 4, 1, file);
	unused = fread(&Rela->r_info, 4, 1, file);
	unused = fread(&Rela->r_offset, 4, 1, file);
}

// ici on parcours les section et l'orsque l'on arrive sur un type rel ou rela on l'affiche
void GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables){
	long position;
	int isReloc=0;
    for (int i = 0; i < SectionsTables->nbSections; i++){

        Elf32_Shdr * currentSectionTable = SectionsTables->TabAllSec[i];
        if (currentSectionTable->sh_type == SHT_REL){
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            for (int j = 0; j < currentSectionTable->sh_size / 8; j++){
                SectionsTables->TabAllRel[j] = malloc(sizeof(Elf32_Rel));
                GetRelocation_rel(file, SectionsTables->TabAllRel[i][j]);
            }
            fseek(file, position, 0);
        }
	
        else if (currentSectionTable->sh_type == SHT_RELA){
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            for (int j = 0; j < currentSectionTable->sh_size / 12; j++){
                SectionsTables->TabAllRela[j] = malloc(sizeof(Elf32_Rela));
                GetRelocation_rela(file, SectionsTables->TabAllRela[i][j]);
            }
            fseek(file, position, 0);
		}
    }
	if (isReloc==0){
		printf("There are no relocations in this file.");
	}
}


void printRelocation(FILE *file, Elf32_Rel *Rel, Elf32_Rela *Rela){
	
    
    printOctet(&Rel->r_offset, 4, 1);
	printf("\t");
   
    printOctet(&Rel->r_info + 1, 4, 1);
    printf("\t");
    printOctet(&Rel->r_info, 4, 1);
    printf("\t%d\t\t", (char)Rel->r_info);


    printOctet(&Rela->r_offset, 4, 1);
    printf("\t");

    printOctet(&Rela->r_info + 1, 4, 1);
    printf("\t");
    printOctet(&Rela->r_info, 4, 1);
    printf("\t%d\t\t", (char)Rela->r_info);

    printOctet(&Rela->r_offset, 4, 1);
    printf("\t\t");

    return;
}