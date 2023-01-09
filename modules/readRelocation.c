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

void offset_rel(FILE *file, Elf32_Rel *rel, char verbose)
{
	unused = fread(&rel->r_offset, 4, 1, file);
	if (verbose)
	{
		printOctet(&rel->r_offset, 4, 1);
		printf("\t");
	}
}

void info_rel(FILE *file, Elf32_Rel *rel, char verbose)
{
	unused = fread(&rel->r_info, 4, 1, file);
	if (verbose)
	{
		printOctet(&rel->r_info + 1, 4, 1);
		printf("\t");
		printOctet(&rel->r_info, 4, 1);
		printf("\t%d\t\t", (char)rel->r_info);
	}
}

void get_relocation_rel(FILE *file, Elf32_Rel *rel, char verbose)
{
	offset_rel(file, rel, verbose);
	info_rel(file, rel, verbose);
}

void offset_rela(FILE *file, Elf32_Rela *rel, char verbose)
{
	unused = fread(&rel->r_offset, 4, 1, file);
	if (verbose)
	{
		printOctet(&rel->r_offset, 4, 1);
		printf("\t");
	}
}

void info_rela(FILE *file, Elf32_Rela *rel, char verbose)
{
	unused = fread(&rel->r_info, 4, 1, file);
	if (verbose)
	{
		printOctet(&rel->r_info + 1, 4, 1);
		printf("\t");
		printOctet(&rel->r_info, 4, 1);
		printf("\t%d\t\t", (char)rel->r_info);
	}
}

void addend_rela(FILE *file, Elf32_Rela *rel, char verbose)
{
	unused = fread(&rel->r_offset, 4, 1, file);
	if (verbose)
	{
		printOctet(&rel->r_offset, 4, 1);
		printf("\t\t");
	}
}

void get_relocation_rela(FILE *file, Elf32_Rela *rela, char verbose)
{
	offset_rela(file, rela, verbose);
	info_rela(file, rela, verbose);
	addend_rela(file, rela, verbose);
}

// ici on parcours les section et l'orsque l'on arrive sur un type rel ou rela on l'affiche
void GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables, Elf32_Rel *rel, Elf32_Rela *rela)
{
	long position;
	int isReloc=0;
    for (int i = 0; i < SectionsTables->nbSections; i++)
    {

        Elf32_Shdr * currentSectionTable = SectionsTables->TabAllSec[i];
        if (currentSectionTable->sh_type == SHT_REL)
        {
			isReloc=1;
            position = ftell(file);

            printf("Relocation section '%s'\n", getStringSection(/*file,*/ currentSectionTable->sh_name, Header, SectionsTables));
            fseek(file, position, 0);
            printf("Offset\t\tsymb\t\tInfo\t\tType\n");
            printf("====================================================================");
            printf("=====================================================================\n");
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            for (int i = 0; i < currentSectionTable->sh_size / 8; i++)
            {
                get_relocation_rel(file, rel, 1);
                printf("\n");
            }
            fseek(file, position, 0);
        }
	
        else if (currentSectionTable->sh_type == SHT_RELA)
        {
		isReloc=1;
            position = ftell(file);
            printf("Relocation section '%s'\n", getStringSection(/*file, */currentSectionTable->sh_name, Header, SectionsTables));
            fseek(file, position, 0);
            printf("Offset\t\tsymb\t\tInfo\t\tType\n");
            printf("====================================================================");
            printf("=====================================================================\n");
            position = ftell(file);
            fseek(file, currentSectionTable->sh_offset, SEEK_SET);
            for (int i = 0; i < currentSectionTable->sh_size / 12; i++)
            {
                get_relocation_rela(file, rela, 1);
                printf("\n");
            }
            fseek(file, position, 0);
		}
    }
	if (isReloc==0){
		printf("There are no relocations in this file.");
	}
}
