//
//  readTableSymbole.c
//  Editeur de Liens
//
#include <stdio.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"



void offset_rel(FILE* file, Elf32_Rel* rel, char verbose)
{
    fread(&rel->r_offset, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_offset, 4, 1);
        printf("\t");
    }
}

void info_rel(FILE* file, Elf32_Rel* rel, char verbose)
{
    fread(&rel->r_info, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_info+1,4,1);
        printf("\t");
        printOctet(&rel->r_info,4,1);
        printf("\t%d\t\t", (char)rel->r_info);
    }
}

void get_relocation_rel(FILE* file, Elf32_Rel* rel, char verbose)
{
    offset_rel(file, rel, verbose);
    info_rel(file, rel, verbose);
}

void offset_rela(FILE* file, Elf32_Rela* rel, char verbose)
{
    fread(&rel->r_offset, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_offset, 4, 1);
        printf("\t");
    }
}

void info_rela(FILE* file, Elf32_Rela* rel, char verbose)
{
    fread(&rel->r_info, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_info+1,4,1);
        printf("\t");
        printOctet(&rel->r_info,4,1);
        printf("\t%d\t\t", (char)rel->r_info);
    }
}

void addend_rela(FILE* file, Elf32_Rela* rel, char verbose)
{
    fread(&rel->r_offset, 4, 1, file);
    if(verbose)
    {
        printOctet(&rel->r_offset, 4, 1);
        printf("\t\t");
    }
}

void get_relocation_rela(FILE* file, Elf32_Rela* rela, char verbose)
{
    offset_rela(file, rela, verbose);
    info_rela(file, rela, verbose);
    addend_rela(file, rela, verbose);
}


//ici on parcours les section et l'orsque l'on arrive sur un type rel ou rela on l'affiche
void GetRelocationPart(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable,Elf32_Rel* rel, Elf32_Rela* rela, char verbose){
	long position;
	char* mot = malloc(50);
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
            if(SectionTable->sh_type == SHT_REL){
				position = ftell(file);
				getString(file, SectionTable->sh_name, Header, mot);
				fseek(file, position, 0);
				printf("relocation section '%s'\n", mot);
				printf("Offset\t\tsymb\t\tInfo\t\tType\n");
				printf("====================================================================");
				printf("=====================================================================\n");
				position = ftell(file);
			    fseek(file, SectionTable->sh_offset, SEEK_SET);
				for(int i = 0; i < SectionTable->sh_size/8; i++){
				get_relocation_rel(file, rel, 1);
				printf("\n");
				}
				fseek(file, position, 0);
			}
            if(SectionTable->sh_type == SHT_RELA){
				position = ftell(file);
				getString(file, SectionTable->sh_name, Header, mot);
				fseek(file, position, 0);
				printf("relocation section '%s'\n", mot);
				printf("Offset\t\tsymb\t\tInfo\t\tType\n");
				printf("====================================================================");
				printf("=====================================================================\n");
				position = ftell(file);
			    fseek(file, SectionTable->sh_offset, SEEK_SET);
				for(int i = 0; i < SectionTable->sh_size/12; i++){
					get_relocation_rela(file, rela, 1);
					printf("\n");
				}
				fseek(file, position, 0);
			}
		}
	}
}