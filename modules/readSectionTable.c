//
//  readSectionTable.c
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 20/12/2022.
//

#include "readSectionTable.h"

#include "readHeader.h"
#include "CustomElf.h"
#include "freadoctet.h"

void sectionName(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_name, 4, 1, file);
		printOctet(&SectionTable->sh_name, 4, 1);
		printf("\t");
		//TODO: Décoder sh_name en nom avec la String Index Table
	}else{
		fread(&SectionTable->sh_name, 4, 1, file);
	}
}


void sectionType(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_type, 4, 1, file);
		switch(SectionTable->sh_type){ //Beaucoup de types à cause de firmware.elf pour éviter des =UNK=
			case(SHT_NULL):
				printf("NULL\t\t");
				break;
			case(SHT_PROGBITS):
				printf("PROGBITS\t");
				break;
			case(SHT_SYMTAB):
				printf("SYMTAB\t\t");
				break;
			case(SHT_STRTAB):
				printf("STRTAB\t\t");
				break;
			case(SHT_RELA):
				printf("RELA\t\t");
				break;
			case(SHT_HASH):
				printf("HASH\t\t");
				break;
			case(SHT_DYNAMIC):
				printf("DYNAMIC\t\t");
				break;
			case(SHT_NOTE):
				printf("NOTE\t\t");
				break;
			case(SHT_NOBITS):
				printf("NO BITS\t\t");
				break;
			case(SHT_REL):
				printf("REL\t\t");
				break;
			case(SHT_SHLIB):
				printf("SHLIB\t\t");
				break;
			case(SHT_LOPROC):
				printf("LOW PROC\t\t");
				break;
			case(SHT_HIPROC):
				printf("HI PROC\t\t");
				break;
			case(SHT_LOUSER):
				printf("LOW USR\t\t");
				break;
			case(SHT_HIUSER):
				printf("HI USR\t\t");
				break;
			case(SHT_PREINIT_ARRAY):
				printf("PREINIT_ARRAY\t");
				break;
			case(SHT_INIT_ARRAY):
				printf("INIT_ARRAY\t");
				break;
			case(SHT_FINI_ARRAY):
				printf("FINI_ARRAY\t");
				break;
			case(SHT_ARM_ATTRIBUTES):
				printf("ARM_ATTRIB\t");
				break;
			default:
				printf("=UNK=\t\t");
				break;
		}
	}else{
		fread(&SectionTable->sh_type, 4, 1, file);
	}
}


void sectionFlags(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_flags, 4, 1, file);
		printf("%d ", SectionTable->sh_flags);
		printf("\t");
		//TODO: Décoder sh_flags en attributs (A, B, C ...) avec la Figure 1-11 page 13
	}else{
		fread(&SectionTable->sh_flags, 4, 1, file);
	}
}


void sectionAdress(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_addr, 4, 1, file);
		printOctet(&SectionTable->sh_addr, 4, 1);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_addr, 4, 1, file);
	}
}


void sectionOffset(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_offset, 4, 1, file);
		printf("%d ", SectionTable->sh_offset);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_offset, 4, 1, file);
	}
}


void sectionSize(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_size, 4, 1, file);
		printf("%d ", SectionTable->sh_size);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_size, 4, 1, file);
	}
}


void sectionLink(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_link, 4, 1, file);
		printf("%d ",SectionTable->sh_link);
		printf("\t");
	}else{
		fread(&SectionTable->sh_link, 4, 1, file);
	}
}


void sectionInfo(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_info, 4, 1, file);
		printf("%d ",SectionTable->sh_info);
		printf("\t");
	}else{
		fread(&SectionTable->sh_info, 4, 1, file);
	}
}


void sectionAdressAlign(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_addralign, 4, 1, file);
		printf("%d ", SectionTable->sh_addralign);
		printf("\t");
	}else{
		fread(&SectionTable->sh_addralign, 4, 1, file);
	}
}


void sectionEntrySize(FILE *file, Elf32_Shdr* SectionTable, char verbose){
	if (verbose){
		fread(&SectionTable->sh_entsize, 4, 1, file);
		printf("%d ", SectionTable->sh_entsize);
		printf("\t");
	}else{
		fread(&SectionTable->sh_entsize, 4, 1, file);
	}
}


void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose){
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
		}
	}
}
