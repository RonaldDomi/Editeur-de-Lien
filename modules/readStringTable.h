//
//  readStringTable.h
//  Editeur-de-lien
//
//  Created by Bastien Levasseur on 29/12/2022.
//

#ifndef readStringTable_h
#define readStringTable_h

char * getStringSection(/*FILE *file, */Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections);

char * getStringSymbol(/*FILE *file, */Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections);

#endif /* readStringTable_h */
