//
//  readStringTable.c
//  Editeur de Liens
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CustomElf.h"

char * getStringSection(Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections)
{
	// fseek(file, Sections->TabAllSec[Header->e_shstrndx]->sh_offset, SEEK_SET);
	// fseek(file, index, SEEK_CUR);
	char *mot = malloc(50);
	int i = index;
	while (Sections->TabAllSecContent[Header->e_shstrndx][i] != 0)
	{
		mot[i-index] = Sections->TabAllSecContent[Header->e_shstrndx][i];
		i++;
	}
	mot[i-index] = Sections->TabAllSecContent[Header->e_shstrndx][i];
	return mot;
	// printf("Result : <%s>", fgets(mot, 50, file));
	// return fgets(mot, 50, file);
}

char * getStringSymbol(Elf32_Word index, Elf32_Ehdr *Header, Elf32_AllSec *Sections)
{
	// fseek(file, Sections->TabAllSec[Header->e_shstrndx]->sh_offset, SEEK_SET);
	// fseek(file, index, SEEK_CUR);
	char *mot = malloc(50);
	int i = index;
	while (Sections->TabAllSecContent[Header->e_shstrndx-1][i] != 0)
	{
		mot[i-index] = Sections->TabAllSecContent[Header->e_shstrndx-1][i];
		i++;
	}
	mot[i-index] = Sections->TabAllSecContent[Header->e_shstrndx-1][i];


	if (strlen(mot)==0){
		mot ="==NO_NAME==";
	}

	return mot;
	// printf("Result : <%s>", fgets(mot, 50, file));
	// return fgets(mot, 50, file);
}
