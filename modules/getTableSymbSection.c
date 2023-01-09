//
//  getTableSymbSection.c
//  Editeur de Liens
//

#include <stdio.h>
#include <string.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"
#include "readStringTable.h"
#include "lecture.h"

int unused; // Var non utilisée pour les warnings lors du make

void decodeSymbShndx(Elf32_Sym * symtab){
	if (symtab->st_shndx==0)
		printf("UND\t");
	else if(symtab->st_shndx==65521)
		printf("ABS\t");
	else
		printf("%d\t", symtab->st_shndx);
}

void decodeSymbType(Elf32_Sym * symtab){
	switch ((symtab->st_info) & 0xf)
		{
			case 0:
			printf("NOTYPE\t\t");
			break;
			case 1:
			printf("OBJECT\t\t");
			break;
			case 2:
			printf("FUNC\t\t");
			break;
			case 3:
			printf("SECTION\t\t");
			break;
			case 4:
			printf("FILE\t\t");
			break;
			case 13:
			printf("LOPROC\t\t");
			break;
			case 15:
			printf("HIPROC\t\t");
			break;

			default:
			break;
		}

	switch ((symtab->st_info) >> 4)
		{
			case 0:
			printf("LOCAL\t");
			break;
			case 1:
			printf("GLOBAL\t");
			break;
			case 2:
			printf("WEAK\t");
			break;
			case 13:
			printf("LOPROC\t");
			break;
			case 15:
			printf("HIPROC\t");
			break;

			default:
			break;
		}
}


void decodeSymbOther(Elf32_Sym * symtab){
	if (symtab->st_other==0){
		printf("DEFAULT\t");
	}
	else printf("%d\t", symtab->st_other);
}


void printTableSymb(Elf32_Sym * symtab, Elf32_AllSec * Sections, Elf32_Ehdr * Header){
	dumpOctet(&symtab->st_value, 4);
	printf("\t");

	printf("%d\t", symtab->st_size);

	decodeSymbType(symtab);

	decodeSymbOther(symtab);

	decodeSymbShndx(symtab);

	printf("%s\t", getStringSymbol(symtab->st_name, Header, Sections));
}

void printAllTableSymb(Elf32_Sym * AllSymbolTables, Elf32_AllSec * AllSectionsTables, Elf32_Ehdr * Header){

    int indexSymbSec = getSectionByType(AllSectionsTables, SHT_SYMTAB);
    Elf32_Shdr * SectionTable = AllSectionsTables->TabAllSec[indexSymbSec];
    
	printf("\n");
	printf("Adresses are given un hexadecimal format.\n");
	printf("All values are given in bytes in decimal format.\n\n");
	printf("Nb\tValue\t\tSize\tType\t\tBind\tOther\tShndx\tName\n");
	printf("====================================================================");
	printf("=====================================================================\n");

	for(int i=0; i<SectionTable->sh_size/16; i++){
		printf("%d:\t", i);
		printTableSymb(AllSymbolTables+i, AllSectionsTables, Header);
        printf("\n");
    }
}


void GetTableSymbPart(Elf32_SecContent SectionContent, Elf32_Sym *symtab, int adrligne){

/* TODO: 3 méthodes, en choisir une et supprimer les deux autres, fixer de préférence sscanf */
		// dump content
		// for(int i=adrligne; i<adrligne+16; i++)
		//     printf("%02x", (unsigned char)SectionContent[i]);
		// printf(" : ");

		// read from sectionContent
		// sscanf(SectionContent+adrligne+0, "%d", &symtab->st_name);
		// sscanf(SectionContent+adrligne+4, "%d", &symtab->st_value);
		// sscanf(SectionContent+adrligne+8, "%d", &symtab->st_size);
		// sscanf(SectionContent+adrligne+12, "%c", &symtab->st_info);
		// sscanf(SectionContent+adrligne+13, "%c", &symtab->st_other);
		// sscanf(SectionContent+adrligne+14, "%hd", &symtab->st_shndx);
		lecture(SectionContent+adrligne+0,  &symtab->st_name, 4);
		lecture(SectionContent+adrligne+4, &symtab->st_value, 4);
		lecture(SectionContent+adrligne+8, &symtab->st_size, 4);
		lecture(SectionContent+adrligne+12, &symtab->st_info, 1);
		lecture(SectionContent+adrligne+13, &symtab->st_other, 1);
		lecture(SectionContent+adrligne+14, &symtab->st_shndx, 2);

		// read from file
	// unused = fread(&symtab->st_name, 4, 1, file);
	// unused = fread(&symtab->st_value, 4, 1, file);
	// unused = fread(&symtab->st_size, 4, 1, file);
	// unused = fread(&symtab->st_info, 1, 1, file);
	// unused = fread(&symtab->st_other, 1, 1, file);
	// unused = fread(&symtab->st_shndx, 2, 1, file);
}

void getAllTableSymb(FILE *file, Elf32_Ehdr *Header, Elf32_SecContent SectionContent, Elf32_Sym *AllSymbolTables, int nbTable){
	for (int i = 0; i < nbTable ; i++)
	{
		GetTableSymbPart(SectionContent, AllSymbolTables+i, i*16);
	}
}
	// ici ecrire les fonction de recuperation du tableSymbole entry

// void symTabName(FILE *file, Elf32_Ehdr *Header, Elf32_Sym *symtab, char verbose)
// {
// 		// if (verbose)
// 		// {
// 		// 	char *mot = malloc(50);
// 		// 	long position = ftell(file);
// 		// 	getString(file, symtab->st_name, Header, mot);
// 		// 	fseek(file, position, 0);
// 		// 	printf("%s\t", mot);
// 		// }
// 	return;
// }
