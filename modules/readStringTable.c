
#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"

void GetTableStringPart(FILE *file, Elf32_Ehdr *Header, Elf32_Shdr *SectionTable)
{
	fseek(file, Header->e_shstrndx * Header->e_shentsize, SEEK_CUR);
	sectionName(file, Header, SectionTable, 0, 0);
	sectionType(file, SectionTable, 0);
	sectionFlags(file, SectionTable, 0);
	sectionAdress(file, SectionTable, 0);
	sectionOffset(file, SectionTable, 0);
	sectionSize(file, SectionTable, 0);
	sectionLink(file, SectionTable, 0);
	sectionInfo(file, SectionTable, 0);
	sectionAdressAlign(file, SectionTable, 0);
	sectionEntrySize(file, SectionTable, 0);
}

void getString(FILE *file, Elf32_Word index, Elf32_Ehdr *Header, char *mot)
{
	Elf32_Shdr *strTabSect = malloc(sizeof(Elf32_Shdr));
	fseek(file, Header->e_shoff, SEEK_SET);
	GetTableStringPart(file, Header, strTabSect);
	fseek(file, strTabSect->sh_offset, SEEK_SET);
	fseek(file, index, SEEK_CUR);
	int i = 0;
	while ((mot[i] = fgetc(file)))
	{
		i++;
	}
}
