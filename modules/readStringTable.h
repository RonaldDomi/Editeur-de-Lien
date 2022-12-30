//
//  readStringTable.h
//  Editeur-de-lien
//
//  Created by Bastien Levasseur on 29/12/2022.
//

#ifndef readStringTable_h
#define readStringTable_h
void getString(FILE* file, Elf32_Word index, Elf32_Ehdr* Header, char* mot);

void GetTableStringPart(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose);


#endif /* readStringTable_h */
