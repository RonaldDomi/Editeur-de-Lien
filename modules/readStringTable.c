
#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"

char * getString(FILE *file, Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections)
{
	fseek(file, Sections->TabAllSec[Header->e_shstrndx]->sh_offset, SEEK_SET);
	fseek(file, index, SEEK_CUR);

    char mot[50];

    return fgets(mot, 50, file);
     
}
