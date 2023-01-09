//
//  readRelocation.h
//  Editeur de Liens
//

#ifndef readRelocation_h
#define readRelocation_h

#include "CustomElf.h"



void GetRelocation_rel(FILE *file, Elf32_Rel *Rel);
void GetRelocation_rela(FILE *file, Elf32_Rela *Rela);
void GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables);
void printRelocation(FILE *file, Elf32_Rel *Rel, Elf32_Rela *Rela);

#endif /* readRelocation_h */
