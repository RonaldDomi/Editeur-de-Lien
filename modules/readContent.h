//
//  readContent.h
//  Editeur-de-lien
//
#ifndef readContent_h
#define readContent_h

#include <stdio.h>
#include "CustomElf.h"

void printContent(FILE* file, Elf32_Shdr* SectionTable, int sectionSelected, char* nameOfSection);

#endif /* readContent_h */
