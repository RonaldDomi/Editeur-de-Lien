//
//  mergeelf.h
//  Editeur-de-lien
//
//  Created by Bastien Levasseur on 03/01/2023.
//

#ifndef mergeelf_h
#define mergeelf_h

#include <stdio.h>
#include "modules/CheckElf.h"
#include "modules/CustomElf.h"
#include "mergeelf.h"

void mergeSymbol(FILE* file, Elf32_Ehdr *Header, Elf32_AllSec * AllSectionsTables, Elf32_SecContent SectionContent1, Elf32_SecContent SectionContent2, int nbTable1, int nbTable2);

#endif /* mergeelf_h */
