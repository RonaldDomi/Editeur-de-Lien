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
    printf("val : %d\n", Rel->r_offset);
    
    unused = fread(&Rel->r_info, 4, 1, file);
}


void GetRelocation_rela(FILE *file, Elf32_Rela *Rela){
	unused = fread(&Rela->r_offset, 4, 1, file);
	unused = fread(&Rela->r_info, 4, 1, file);
	unused = fread(&Rela->r_offset, 4, 1, file);
}

// ici on parcours les section et l'orsque l'on arrive sur un type rel ou rela on l'affiche
int GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables){
	long position;
    int isReloc = 0;
    for (int i = 0; i < SectionsTables->nbSections; i++){
        Elf32_Shdr * currentSectionTable = SectionsTables->TabAllSec[i];
        if (currentSectionTable->sh_type == SHT_REL){
            printf("Relocation section '%s'\n", getStringSection(currentSectionTable->sh_name, Header, SectionsTables));
            isReloc = 1;
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            SectionsTables->TabAllRel[i] = malloc(currentSectionTable->sh_size / 12);
            for (int j = 0; j < currentSectionTable->sh_size / 8; j++){
                SectionsTables->TabAllRel[i][j] = malloc(sizeof(Elf32_Rel));
                GetRelocation_rel(file, SectionsTables->TabAllRel[i][j]);
            }
            fseek(file, position, 0);
        }
	
        else if (currentSectionTable->sh_type == SHT_RELA){
            printf("Relocation section '%s'\n", getStringSection(currentSectionTable->sh_name, Header, SectionsTables));
            isReloc = 1;
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            SectionsTables->TabAllRela[i] = malloc(currentSectionTable->sh_size / 12);
            for (int j = 0; j < currentSectionTable->sh_size / 12; j++){
                SectionsTables->TabAllRela[i][j] = malloc(sizeof(Elf32_Rela));
                GetRelocation_rela(file, SectionsTables->TabAllRela[i][j]);
            }
            fseek(file, position, 0);
		}
    }
    return isReloc;
}


void printRelocation(FILE *file, Elf32_AllSec * SectionsTables, int isReloc){
    if (isReloc==0){
		printf("There are no relocations in this file.");
        return;
    }
	for (int i = 0; i < SectionsTables->nbSections; i++){
        if (SectionsTables->TabAllRel[i]!=NULL){
            printf("Offset\t\tsymb\t\tInfo\t\tType\n");
            printf("====================================================================");
            printf("=====================================================================\n");
            for (int j = 0; j < SectionsTables->TabAllSec[i]->sh_size / 8; j++){
                dumpOctet(&SectionsTables->TabAllRel[i][j]->r_offset, 4);
                printf("\t");
            
                dumpOctet(&SectionsTables->TabAllRel[i][j]->r_info + 1, 4);
                printf("\t");
                dumpOctet(&SectionsTables->TabAllRel[i][j]->r_info, 4);
                printf("\t%d\t\t\n", (char)SectionsTables->TabAllRel[i][j]->r_info); 
            }
            printf("\n");
        }
        else if (SectionsTables->TabAllRela[i]!=NULL){
            printf("Offset\t\tsymb\t\tInfo\t\tType\n");
            printf("====================================================================");
            printf("=====================================================================\n");
            for (int j = 0; j < SectionsTables->TabAllSec[i]->sh_size / 8; j++){
                dumpOctet(&SectionsTables->TabAllRela[i][j]->r_offset, 4);
                printf("\t");

                dumpOctet(&SectionsTables->TabAllRela[i][j]->r_info + 1, 4);
                printf("\t");
                dumpOctet(&SectionsTables->TabAllRela[i][j]->r_info, 4);
                printf("\t%d\t\t", (char)SectionsTables->TabAllRela[i][j]->r_info);

                dumpOctet(&SectionsTables->TabAllRela[i][j]->r_offset, 4);
                printf("\t\t\n");
            }
            printf("\n");
        }
    }
}