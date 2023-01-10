#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readAll.h"
#include "modules/freadoctet.h"
#include "create_elf.h"

// void write_to_file(Elf32_Info*, FILE*);
// void write_header(Elf32_Ehdr*, FILE*);
// void write_sections(Elf32_AllSec*, FILE*);
// void write_section_header(Elf32_AllSec*, FILE*);


// int main(int argc, char **argv){

// 	if (argc<2){
// 		printf("Usage : ./create_elf <File1>\n");
// 		exit(1);
// 	}

// 	FILE *file1 = fopen(argv[1],"rb");
// 	// FILE *file2 = fopen(argv[2],"rb");
// 	FILE *file2 = fopen("creation.o","w+");

// 	if (file1==NULL){
// 		printf("ERROR : %s no such file.\n", argv[1]);
// 		exit(1);
// 	}

// 	if (!checkELF(file1)){
// 		printf("Not a ELF file !\n");
// 		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
// 		exit(1);
// 	}

// 	Elf32_Info * ELF1 = getAllInfo(file1);

//     write_to_file(ELF1, file2);
    
// 	fclose(file1);
// 	fclose(file2);
// }



void write_header(Elf32_Ehdr* Header, FILE* file){
    printf("################## WRITING HEADER BYTES ##############\n\n");

    // printf("ident: ");
    // printOctet(&Header->e_ident, 16, 0);
    fwrite(&Header->e_ident, 1, 16, file);
    // printf("\n");

    // printf("type: ");
    // printOctet(&Header->e_type, 2, 0);
    fwrite(&Header->e_type, 1, 2, file);
    // printf("\n");
    
    // printf("e_machine: ");
    // printOctet(&Header->e_machine, 2, 0);
    fwrite(&Header->e_machine, 1, 2, file);
    // printf("\n");
    
    // printf("e_version: ");
    // printOctet(&Header->e_version, 4, 0);
    fwrite(&Header->e_version, 1, 4, file);
    // printf("\n");
    
    // printf("e_entry: ");
    // printOctet(&Header->e_entry, 4, 0);
    fwrite(&Header->e_entry, 1, 4, file);
    // printf("\n");
    
    // printf("e_phoff: ");
    // printOctet(&Header->e_phoff, 4, 0);
    fwrite(&Header->e_phoff, 1, 4, file);
    // printf("\n");
    
    // printf("e_shoff: ");
    // printOctet(&Header->e_shoff, 4, 0);
    fwrite(&Header->e_shoff, 1, 4, file);
    // printf("\n");
    
    // printf("e_flags: ");
    // printOctet(&Header->e_flags, 4, 0);
    fwrite(&Header->e_flags, 1, 4, file);
    // printf("\n");
    
    // printf("e_ehsize: ");
    // printOctet(&Header->e_ehsize, 2, 0);
    fwrite(&Header->e_ehsize, 1, 2, file);
    // printf("\n");
    
    // printf("e_phentsize: ");
    // printOctet(&Header->e_phentsize, 2, 0);
    fwrite(&Header->e_phentsize, 1, 2, file);
    // printf("\n");
    
    // printf("e_phnum: ");
    // printOctet(&Header->e_phnum, 2, 0);
    fwrite(&Header->e_phnum, 1, 2, file);
    // printf("\n");
    
    // printf("e_shentsize: ");
    // printOctet(&Header->e_shentsize, 2, 0);
    fwrite(&Header->e_shentsize, 1, 2, file);
    // printf("\n");
    
    // printf("e_shnum: ");
    // printOctet(&Header->e_shnum, 2, 0);
    fwrite(&Header->e_shnum, 1, 2, file);
    // printf("\n");
    
    // printf("e_shstrndx: ");
    // printOctet(&Header->e_shstrndx, 2, 0);
    fwrite(&Header->e_shstrndx, 1, 2, file);
    // printf("\n");
    
}

void write_sections(Elf32_AllSec* Sections, FILE* file){
    printf("\n################## WRITING SECTION BYTES ##############\n\n");
    // printf("NB sECTIONS %d\n", Sections->nbSections);
    // printf("offset of Sections->TabAllSec[23]->sh_offset %d\n", Sections->TabAllSec[23]->sh_offset);
    // fprintf(stderr,"offset of Sections->TabAllSec[24]->sh_offset %d\n", Sections->TabAllSec[24]->sh_size);
    // get the write order of sections to write into order
    int present[Sections->nbSections];
    int order[Sections->nbSections];
    for(int i = 0; i<Sections->nbSections; i++){
        present[i] = 0;
        order[i] = i;
    }

    int min_pos = 0;
    int min;
    for(int i=0; i<Sections->nbSections; i++){
        min = 1000000;
        min_pos = i;
        for(int j=0; j<Sections->nbSections; j++){
            // fprintf(stderr, "Hello2 %d %d\n", Sections->nbSections, j);
            // fprintf(stderr, "offset: %d\n\n", Sections->TabAllSec[j]->sh_offset);
            if(Sections->TabAllSec[j]->sh_offset < min){
                // fprintf(stderr, "Hello3");
                // si il est pas present
                if(present[j] == 0){
                    min = Sections->TabAllSec[j]->sh_offset;
                    min_pos = j;
                    // fprintf(stderr, "Hello3");
                }
            }
        }
        present[min_pos] = 1;
        order[i] = min_pos;
    }

    for(int i = 0; i<Sections->nbSections; i++){
        int size = Sections->TabAllSec[order[i]]->sh_size;
        
        if(size != 0){
            fwrite(Sections->TabAllSecContent[order[i]], 1, size, file);
            if(i+2 <= Sections->nbSections){
                // difference is to know if we need buffer 0 so that the alignments match
                int difference = Sections->TabAllSec[order[i+1]]->sh_offset - (Sections->TabAllSec[order[i]]->sh_offset + Sections->TabAllSec[order[i]]->sh_size);
                if(difference > 0){
                    // complete with additional 0
                    while(difference > 0){
                        int output[2] = {0, 0};
                        fwrite(output, 1, 1, file);
                        difference = difference - 1;
                    }
                }
            }
        }
    }
}

void write_section_header(Elf32_AllSec* Sections, FILE* file){
    printf("\n################## WRITING SECTION HEADER BYTES ##############\n\n");
    for(int i = 0; i<Sections->nbSections; i++){
        Elf32_Shdr* SectionTable = Sections->TabAllSec[i];
        // printf("sh_name: ");
        // printOctet(&SectionTable->sh_name, 4, 0);
        fwrite(&SectionTable->sh_name, 1, 4, file);
        // printf("\n");

        // printf("sh_type: ");
        // printOctet(&SectionTable->sh_type, 4, 0);
        fwrite(&SectionTable->sh_type, 1, 4, file);
        // printf("\n");
        
        // printf("sh_flags: ");
        // printOctet(&SectionTable->sh_flags, 4, 0);
        fwrite(&SectionTable->sh_flags, 1, 4, file);
        // printf("\n");
        
        // printf("sh_addr: ");
        // printOctet(&SectionTable->sh_addr, 4, 0);
        fwrite(&SectionTable->sh_addr, 1, 4, file);
        // printf("\n");
        
        // printf("sh_offset: ");
        // printOctet(&SectionTable->sh_offset, 4, 0);
        fwrite(&SectionTable->sh_offset, 1, 4, file);
        // printf("\n");
        
        // printf("sh_size: ");
        // printOctet(&SectionTable->sh_size, 4, 0);
        fwrite(&SectionTable->sh_size, 1, 4, file);
        // printf("\n");
        
        // printf("sh_link: ");
        // printOctet(&SectionTable->sh_link, 4, 0);
        fwrite(&SectionTable->sh_link, 1, 4, file);
        // printf("\n");
        
        // printf("sh_info: ");
        // printOctet(&SectionTable->sh_info, 4, 0);
        fwrite(&SectionTable->sh_info, 1, 4, file);
        // printf("\n");
        
        // printf("sh_addralign: ");
        // printOctet(&SectionTable->sh_addralign, 4, 0);
        fwrite(&SectionTable->sh_addralign, 1, 4, file);
        // printf("\n");
        
        // printf("sh_entsize: ");
        // printOctet(&SectionTable->sh_entsize, 4, 0);
        fwrite(&SectionTable->sh_entsize, 1, 4, file);
        // printf("\n");
    }

}

void write_to_file(Elf32_Info* ELF1, FILE* file){
    write_header(ELF1->Header, file);
    write_sections(ELF1->AllSections, file);
    
    // complete resting 0
    long current_pos = ftell(file);
    while(current_pos < ELF1->Header->e_shoff){

        int output[2] = {0, 0};
        fwrite(output, 1, 1, file);
        // printf("adding one 0 ");
        current_pos = ftell(file);
    }


    write_section_header(ELF1->AllSections, file);
}