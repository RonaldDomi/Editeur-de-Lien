//
//  readRelocation.h
//  Editeur de Liens
//

#ifndef readRelocation_h
#define readRelocation_h

#include "CustomElf.h"

void offset_rel(FILE *file, Elf32_Rel *rel, char verbose);


void info_rel(FILE *file, Elf32_Rel *rel, char verbose);


void get_relocation_rel(FILE *file, Elf32_Rel *rel, char verbose);


void offset_rela(FILE *file, Elf32_Rela *rel, char verbose);


void offset_rela(FILE *file, Elf32_Rela *rel, char verbose);

void GetRelocationPart(FILE *file, Elf32_Ehdr *Header, Elf32_AllSec * SectionsTables, Elf32_Rel *rel, Elf32_Rela *rela);
#endif /* readRelocation_h */
