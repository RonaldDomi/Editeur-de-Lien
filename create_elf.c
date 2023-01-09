#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readAll.h"
#include "modules/freadoctet.h"

void write_to_file(Elf32_Info*, FILE*);
void write_header(Elf32_Ehdr*, FILE*);
void write_sections(Elf32_AllSec*, FILE*);
void write_section_header(Elf32_AllSec*, FILE*);


int main(int argc, char **argv){

	if (argc<2){
		printf("Usage : ./create_elf <File1>\n");
		exit(1);
	}

	FILE *file1 = fopen(argv[1],"rb");
	// FILE *file2 = fopen(argv[2],"rb");
	FILE *file2 = fopen("creation.o","w+");

	if (file1==NULL){
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	if (!checkELF(file1)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	Elf32_Info * ELF1 = getAllInfo(file1);

    write_to_file(ELF1, file2);
    
	fclose(file1);
	fclose(file2);
}

// Elf32_SecContent
// Elf32_Shdr
// typedef struct {
//     int nbSections;
//     Elf32_Shdr * TabAllSec[50];
//     Elf32_SecContent TabAllSecContent[50];
// } Elf32_AllSec;

// typedef struct {
//     Elf32_Ehdr * Header;
//     Elf32_AllSec * AllSections;
//     Elf32_Sym * AllSymbol;
// } Elf32_Info;
void write_to_file(Elf32_Info* ELF1, FILE* file){
    write_header(ELF1->Header, file);
    write_sections(ELF1->AllSections, file);
    
    // complete resting 0
    long current_pos = ftell(file);
    while(current_pos < ELF1->Header->e_shoff){

        int output[2] = {0, 0};
        fwrite(output, 1, 1, file);
        current_pos = ftell(file);
    }


    write_section_header(ELF1->AllSections, file);
}

void write_header(Elf32_Ehdr* Header, FILE* file){
    printf("################## WRITING HEADER BYTES ##############\n\n");

    printf("ident: ");
    printOctet(&Header->e_ident, 16, 0);
    fwrite(&Header->e_ident, 1, 16, file);
    printf("\n");

    printf("type: ");
    printOctet(&Header->e_type, 2, 0);
    fwrite(&Header->e_type, 1, 2, file);
    printf("\n");
    
    printf("e_machine: ");
    printOctet(&Header->e_machine, 2, 0);
    fwrite(&Header->e_machine, 1, 2, file);
    printf("\n");
    
    printf("e_version: ");
    printOctet(&Header->e_version, 4, 0);
    fwrite(&Header->e_version, 1, 4, file);
    printf("\n");
    
    printf("e_entry: ");
    printOctet(&Header->e_entry, 4, 0);
    fwrite(&Header->e_entry, 1, 4, file);
    printf("\n");
    
    printf("e_phoff: ");
    printOctet(&Header->e_phoff, 4, 0);
    fwrite(&Header->e_phoff, 1, 4, file);
    printf("\n");
    
    printf("e_shoff: ");
    printOctet(&Header->e_shoff, 4, 0);
    fwrite(&Header->e_shoff, 1, 4, file);
    printf("\n");
    
    printf("e_flags: ");
    printOctet(&Header->e_flags, 4, 0);
    fwrite(&Header->e_flags, 1, 4, file);
    printf("\n");
    
    printf("e_ehsize: ");
    printOctet(&Header->e_ehsize, 2, 0);
    fwrite(&Header->e_ehsize, 1, 2, file);
    printf("\n");
    
    printf("e_phentsize: ");
    printOctet(&Header->e_phentsize, 2, 0);
    fwrite(&Header->e_phentsize, 1, 2, file);
    printf("\n");
    
    printf("e_phnum: ");
    printOctet(&Header->e_phnum, 2, 0);
    fwrite(&Header->e_phnum, 1, 2, file);
    printf("\n");
    
    printf("e_shentsize: ");
    printOctet(&Header->e_shentsize, 2, 0);
    fwrite(&Header->e_shentsize, 1, 2, file);
    printf("\n");
    
    printf("e_shnum: ");
    printOctet(&Header->e_shnum, 2, 0);
    fwrite(&Header->e_shnum, 1, 2, file);
    printf("\n");
    
    printf("e_shstrndx: ");
    printOctet(&Header->e_shstrndx, 2, 0);
    fwrite(&Header->e_shstrndx, 1, 2, file);
    printf("\n");
    
}

void write_sections(Elf32_AllSec* Sections, FILE* file){
    printf("\n################## WRITING HEADER BYTES ##############\n\n");

    for(int i = 0; i<Sections->nbSections; i++){
        printf("\nSize of section %d, %d\n", i, Sections->TabAllSec[i]->sh_size);
        int size = Sections->TabAllSec[i]->sh_size;
        if(size != 0){
            fwrite(Sections->TabAllSecContent[i], 1, size, file);
            printOctet(Sections->TabAllSecContent[i], size, 0);
            printf("\n");
        }
    }
}

void write_section_header(Elf32_AllSec* Sections, FILE* file){
    printf("\n################## WRITING SECTION HEADER BYTES ##############\n\n");
    for(int i = 0; i<Sections->nbSections; i++){
        Elf32_Shdr* SectionTable = Sections->TabAllSec[i];
        printf("sh_name: ");
        printOctet(&SectionTable->sh_name, 4, 0);
        fwrite(&SectionTable->sh_name, 1, 4, file);
        printf("\n");

        printf("sh_type: ");
        printOctet(&SectionTable->sh_type, 4, 0);
        fwrite(&SectionTable->sh_type, 1, 4, file);
        printf("\n");
        
        printf("sh_flags: ");
        printOctet(&SectionTable->sh_flags, 4, 0);
        fwrite(&SectionTable->sh_flags, 1, 4, file);
        printf("\n");
        
        printf("sh_addr: ");
        printOctet(&SectionTable->sh_addr, 4, 0);
        fwrite(&SectionTable->sh_addr, 1, 4, file);
        printf("\n");
        
        printf("sh_offset: ");
        printOctet(&SectionTable->sh_offset, 4, 0);
        fwrite(&SectionTable->sh_offset, 1, 4, file);
        printf("\n");
        
        printf("sh_size: ");
        printOctet(&SectionTable->sh_size, 4, 0);
        fwrite(&SectionTable->sh_size, 1, 4, file);
        printf("\n");
        
        printf("sh_link: ");
        printOctet(&SectionTable->sh_link, 4, 0);
        fwrite(&SectionTable->sh_link, 1, 4, file);
        printf("\n");
        
        printf("sh_info: ");
        printOctet(&SectionTable->sh_info, 4, 0);
        fwrite(&SectionTable->sh_info, 1, 4, file);
        printf("\n");
        
        printf("sh_addralign: ");
        printOctet(&SectionTable->sh_addralign, 4, 0);
        fwrite(&SectionTable->sh_addralign, 1, 4, file);
        printf("\n");
        
        printf("sh_entsize: ");
        printOctet(&SectionTable->sh_entsize, 4, 0);
        fwrite(&SectionTable->sh_entsize, 1, 4, file);
        printf("\n");
    }

}
