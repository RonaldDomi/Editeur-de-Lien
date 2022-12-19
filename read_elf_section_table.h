//
//  read_elf_section_table.h
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 18/12/2022.
//

#ifndef read_elf_section_table_h
#define read_elf_section_table_h

#include <stdio.h>
#include "modules/CustomElf.h"

void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);

#endif /* read_elf_section_table_h */
