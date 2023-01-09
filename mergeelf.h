//
//  mergeelf.h
//  Editeur-de-lien
//

#ifndef mergeelf_h
#define mergeelf_h

#include <stdio.h>
#include "modules/CustomElf.h"

void mergeSymbol(FILE* file, Elf32_Ehdr *Header1, Elf32_Ehdr *Header2, Elf32_AllSec * AllSectionsTables1, Elf32_AllSec * AllSectionsTables2, Elf32_SecContent SectionContent1, Elf32_SecContent SectionContent2, int nbTable1, int nbTable2, int* offset);

#endif /* mergeelf_h */
