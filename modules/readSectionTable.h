//
//  readSectionTable.h
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 20/12/2022.
//

#ifndef readSectionTable_h
#define readSectionTable_h

#include <stdio.h>
#include "CustomElf.h"

void sectionName(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);


void sectionType(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionFlags(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionAdress(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionOffset(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionSize(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionLink(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionInfo(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionAdressAlign(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void sectionEntrySize(FILE *file, Elf32_Shdr* SectionTable, char verbose);


void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);
#endif /* readSectionTable_h */
