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


void decodeRelType(Elf32_Word type){
	switch(type){
		case(R_ARM_NONE):
			printf("R_ARM_NONE");
			break;
		case(R_ARM_ABS32):
			printf("R_ARM_ABS32");
			break;
		case(R_ARM_TLS_DESC):
			printf("R_ARM_TLS_DESC");
			break;
		case(R_ARM_COPY):
			printf("R_ARM_COPY");
			break;
		case(R_ARM_RELATIVE):
			printf("R_ARM_RELATIVE");
			break;
		case(R_ARM_PLT32):
			printf("R_ARM_PLT32");
			break;
		case(R_ARM_PREL31):
			printf("R_ARM_PREL31");
			break;
		case(R_ARM_CALL):
			printf("R_ARM_CALL");
			break;
		default:
			printf("==UKN, %d==", type);
			break;
	}
	printf("\t");
}


void GetRelocation_rel(FILE *file, Elf32_Rel *Rel){
	unused = fread(&Rel->r_offset, 4, 1, file);
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

			isReloc = 1;
			position = ftell(file);
			fseek(file, currentSectionTable->sh_offset, SEEK_SET);
			SectionsTables->TabAllRel[i] = malloc(sizeof(void*) * (currentSectionTable->sh_size / 8));
			for (int j = 0; j < currentSectionTable->sh_size / 8; j++){
				SectionsTables->TabAllRel[i][j] = malloc(sizeof(Elf32_Rel));
				GetRelocation_rel(file, SectionsTables->TabAllRel[i][j]);
			}
			fseek(file, position, 0);

		}else if (currentSectionTable->sh_type == SHT_RELA){

			isReloc = 1;
			position = ftell(file);
			fseek(file, currentSectionTable->sh_offset, SEEK_SET);
			SectionsTables->TabAllRela[i] = malloc(sizeof(void*) * (currentSectionTable->sh_size / 12));
			for (int j = 0; j < currentSectionTable->sh_size / 12; j++){
				SectionsTables->TabAllRela[i][j] = malloc(sizeof(Elf32_Rela));
				GetRelocation_rela(file, SectionsTables->TabAllRela[i][j]);
			}
			fseek(file, position, 0);
		}

	}
	return isReloc;
}


void printRelocation(Elf32_AllSec * SectionsTables, int isReloc, Elf32_Ehdr * Header){
	if (isReloc==0){
		printf("There are no relocations in this file.");
		return;
	}
	for (int i = 0; i < SectionsTables->nbSections; i++){
		if (SectionsTables->TabAllRel[i]!=NULL){

            printf("Relocation section '%s' at offset 0x", getStringSection(SectionsTables->TabAllSec[i]->sh_name, Header, SectionsTables));
            printf("%x", SectionsTables->TabAllSec[i]->sh_offset);
            printf(" contains %d entries:\n", SectionsTables->TabAllSec[i]->sh_size / 8);

			printf("=========================================================\n");
			for (int j = 0; j < SectionsTables->TabAllSec[i]->sh_size / 8; j++){
				dumpOctet(&SectionsTables->TabAllRel[i][j]->r_offset, 4);
				printf("\t");
				dumpOctet(&SectionsTables->TabAllRel[i][j]->r_info, 4);
				printf("\t");
				decodeRelType((char) SectionsTables->TabAllRel[i][j]->r_info);
				dumpOctet(&SectionsTables->TabAllRel[i][j]->r_info + 1, 4);
				printf("\n");
			}
			printf("\n");
		}else if (SectionsTables->TabAllRela[i]!=NULL){

            printf("Relocation section '%s' at offset 0x", getStringSection(SectionsTables->TabAllSec[i]->sh_name, Header, SectionsTables));
            printf("%x", SectionsTables->TabAllSec[i]->sh_offset);
            printf(" contains %d entries:\n", SectionsTables->TabAllSec[i]->sh_size / 12);

			printf("Offset\t\tInfo\t\tType\tSymb\n");
			printf("=================================================\n");
			for (int j = 0; j < SectionsTables->TabAllSec[i]->sh_size / 12; j++){
				dumpOctet(&SectionsTables->TabAllRela[i][j]->r_offset, 4);
				printf("\t");
				decodeRelType((char) SectionsTables->TabAllRel[i][j]->r_info);
				dumpOctet(&SectionsTables->TabAllRela[i][j]->r_info + 1, 4);
				printf("\t");
				dumpOctet(&SectionsTables->TabAllRela[i][j]->r_offset, 4);
				printf("\n");
			}
			printf("\n");
		}
	}
}
