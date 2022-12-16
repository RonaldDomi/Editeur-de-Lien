//
//  read_elf_header.h
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 16/12/2022.
//

#ifndef read_elf_header_h
#define read_elf_header_h
int checkELF(FILE *file);


void magicNumber(FILE *file, Elf32_Ehdr* Header);


void objectType(FILE *file, Elf32_Ehdr* Header);


void machineType(FILE *file, Elf32_Ehdr* Header);


void version(FILE *file, Elf32_Ehdr* Header);


void entry(FILE *file, Elf32_Ehdr* Header);


void progHeaderOff(FILE *file, Elf32_Ehdr* Header);


void sectHeaderOff(FILE *file, Elf32_Ehdr* Header);


void flags(FILE *file, Elf32_Ehdr *Header);


void headerSize(FILE *file, Elf32_Ehdr *Header);


void headerProgramSize(FILE *file, Elf32_Ehdr *Header);


void nbProgramHeaders(FILE *file,Elf32_Ehdr *Header);


void sizeSectionHeaders(FILE *file, Elf32_Ehdr *Header);


void nbSectionHeader(FILE *file, Elf32_Ehdr *Header);


void indexStringHeader(FILE *file, Elf32_Ehdr *Header);
#endif /* read_elf_header_h */
