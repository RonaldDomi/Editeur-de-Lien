//
//  readRelocation.h
//  Editeur de Liens
//

#ifndef readRelocation_h
#define readRelocation_h

#include "CustomElf.h"


void decodeRelType(Elf32_Word type);
void GetRelocation_rel(FILE *file, Elf32_Rel *Rel);
void GetRelocation_rela(FILE *file, Elf32_Rela *Rela);
int GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables);
void printRelocation(Elf32_AllSec * SectionsTables, int isReloc, Elf32_Ehdr * Header);

#endif /* readRelocation_h */
