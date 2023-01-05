//
//  readTableSymbole.c
//  Editeur de Liens
//
#include <stdio.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"
#include "readStringTable.h"

int unused; // Var non utilisée pour les warnings lors du make

void decodeSymbType(Elf32_Sym * symtab){
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

switch ((symtab->st_info) & 0xf)
    {
    case 0:
        printf("NO TYPE\t\t");
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
}

void printTableSymb(Elf32_Sym * symtab){

    printOctet(&symtab->st_value, 4, 1);
    printf("\t");

    decodeSymbType(symtab);

    printf("\n");

    printf("%d\t", symtab->st_size);
    
    printf("%d\t", symtab->st_other);

    printf("%d\t", symtab->st_shndx);
}

void GetTableSymbPart(FILE *file, Elf32_Ehdr *Header, Elf32_SecContent * SectionContent, Elf32_Sym *symTab){
    
    if (Header->e_shnum == 0)
	{
        printf("No section Table...\n");
	}
	else
	{
        for(int i=0; i<15; i++)
            printf("%02x", SectionContent[i]);

        // unused = fread(&symtab->st_name, 4, 1, file);
        // unused = fread(&symtab->st_value, 4, 1, file);
        // unused = fread(&symtab->st_size, 4, 1, file);
        // unused = fread(&symtab->st_info, 1, 1, file);
        // unused = fread(&symtab->st_shndx, 2, 1, file);
    }
}

// ici ecrire les fonction de recuperation du tableSymbole entry

void symTabName(FILE *file, Elf32_Ehdr *Header, Elf32_Sym *symtab, char verbose)
{
	// if (verbose)
	// {
	// 	char *mot = malloc(50);
	// 	long position = ftell(file);
	// 	getString(file, symtab->st_name, Header, mot);
	// 	fseek(file, position, 0);
	// 	printf("%s\t", mot);
	// }
    return;
}
