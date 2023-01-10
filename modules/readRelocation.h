//
//  readRelocation.h
//  Editeur de Liens
//

#ifndef readRelocation_h
#define readRelocation_h

#include "CustomElf.h"


void decodeRelType(Elf32_Word type);
void GetRelocation_rel(Elf32_SecContent SectionContent, Elf32_Rel *Rel, int adrligne);
void GetRelocation_rela(Elf32_SecContent SectionContent, Elf32_Rela *Rela, int adrligne);
int GetRelocationPart(Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables);
void printRelocation(Elf32_AllSec * SectionsTables, int isReloc, Elf32_Ehdr * Header);

#endif /* readRelocation_h */
