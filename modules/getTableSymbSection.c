//
//  readTableSymbole.c
//  Editeur de Liens
//
#include <stdio.h>
#include "CustomElf.h"
#include "readSectionTable.h"
#include "freadoctet.h"
#include "readStringTable.h"

void GetTableSymbPart(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
	if (Header->e_shnum == 0){
		if (verbose){
			printf("No section Table...\n");
		}
	}else{
		for (int i=0; i<Header->e_shnum; i++){
			if (verbose){
				printf("[%d]\t", i);
			}
			sectionName(file, Header, SectionTable, verbose);
			sectionType(file, SectionTable, verbose);
			sectionFlags(file, SectionTable, verbose);
			sectionAdress(file, SectionTable, verbose);
			sectionOffset(file, SectionTable, verbose);
			sectionSize(file, SectionTable, verbose);
			sectionLink(file, SectionTable, verbose);
			sectionInfo(file, SectionTable, verbose);
			sectionAdressAlign(file, SectionTable, verbose);
			sectionEntrySize(file, SectionTable, verbose);
			
			if (verbose){
				printf("\n");
			}
            if(SectionTable->sh_type == SHT_SYMTAB) return;
		}
	}
}

//ici ecrire les fonction de recuperation du tableSymbole entry

void symTabName(FILE* file, Elf32_Ehdr* Header, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_name, 4, 1, file);
	if (verbose)
	{
		char* mot = malloc(50);
		long position = ftell(file);
		getString(file, symtab->st_name, Header, mot);
		fseek(file, position, 0);
		printf("%s\t", mot);
	}
}

void symTabValue(FILE* file, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_value, 4, 1, file);
	if(verbose){
		printOctet(&symtab->st_value, 4, 1);
		printf("\t");
	}
}

void symTabSize(FILE* file, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_size, 4, 1, file);
	if(verbose){
		printf("%d\t", symtab->st_size);
	}
}

void symTabInfo(FILE* file, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_info, 1, 1, file);
	if(verbose){
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
}

void symTabOther(FILE* file, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_other, 1, 1, file);
	if(verbose){
		printf("%d\t", symtab->st_other);
	}
}	

void symTabShndx(FILE* file, Elf32_Sym* symtab, char verbose){
	fread(&symtab->st_shndx, 2, 1, file);
	if(verbose){
		printf("%d\t", symtab->st_shndx);
	}
}

void getTabSymb(FILE* file, Elf32_Ehdr* Header, Elf32_Sym* symtab, char verbose){
	symTabName(file, Header, symtab, verbose);
	symTabValue(file, symtab, verbose);
	symTabSize(file, symtab, verbose);
	symTabInfo(file, symtab, verbose);
	symTabOther(file, symtab, verbose);
	symTabShndx(file, symtab, verbose);
}

