//
//  read_elf_header.h
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 16/12/2022.
//

#ifndef read_elf_header_h
#define read_elf_header_h
#include <stdio.h>
#include "modules/CustomElf.h"

int checkELF(FILE *file);


void magicNumber(FILE *file, Elf32_Ehdr* Header, char verbose);


void objectType(FILE *file, Elf32_Ehdr* Header, char verbose);


void machineType(FILE *file, Elf32_Ehdr* Header, char verbose);


void version(FILE *file, Elf32_Ehdr* Header, char verbose);


void entry(FILE *file, Elf32_Ehdr* Header, char verbose);


void progHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose);


void sectHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose);


void flags(FILE *file, Elf32_Ehdr *Header, char verbose);


void headerSize(FILE *file, Elf32_Ehdr *Header, char verbose);


void headerProgramSize(FILE *file, Elf32_Ehdr *Header, char verbose);


void nbProgramHeaders(FILE *file,Elf32_Ehdr *Header, char verbose);


void sizeSectionHeaders(FILE *file, Elf32_Ehdr *Header, char verbose);


void nbSectionHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


void indexStringHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


void getHeader(FILE *file, Elf32_Ehdr *Header, char verbose);


#endif /* read_elf_header_h */
