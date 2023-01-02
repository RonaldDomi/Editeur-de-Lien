#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules/CheckElf.h"
#include "modules/CustomElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/getTableSymbSection.c"
#include "modules/readRelocation.c"



void usage(char *name) {
	fprintf(stderr, "Usage:\n"
		"%s [ --help ] [ --file file ] [ --type h|S|x|s|r ] [ --arg number/string ] file\n\n"
		"Prints the elf structure of the file passed as first argument.\n "
		, name);
}

int main(int argc, char *argv[]) {
    // ###### Getting Arguments ######
    // printf("argc: %d\n", argc);
    if(argc < 5){
        usage(argv[0]);
        exit(0);
    }

	int opt;
	char *filename, *type, *arg;

	struct option longopts[] = {
		{ "filename", required_argument, NULL, '1' },
		{ "type", required_argument, NULL, '2' },
		{ "arg", required_argument, NULL, '3' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL, 0, NULL, 0 }
	};

	filename = NULL;
	type = NULL;
	arg = NULL;
	while ((opt = getopt_long(argc, argv, "1:2:3:h", longopts, NULL)) != -1) {
		switch(opt) {
		case '1':
			filename = optarg;
			break;
		case '2':
			type = optarg;
			break;
		case '3':
            arg = optarg;
			break;
		case 'h':
			usage(argv[0]);
			exit(0);
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			usage(argv[0]);
			exit(1);
		}
	}

    // ###### Starting file control ######
	FILE *file = fopen(filename,"rb\n");
	if (file==NULL){
		printf("ERROR : %s no such file.\n", filename);
		exit(1);
	}

	if (!checkELF(file)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

    // ###### Starting readelf control ######
    // correct type
    if(strcmp(type, "h") != 0 && \
        strcmp(type, "S") != 0 && \
        strcmp(type, "x") != 0 && \
        strcmp(type, "s") != 0 && \
        strcmp(type, "r") != 0){
            printf("--type %s is not recognized, please retry with h|S|x|s|r \n", type);
            exit(0);
    }

    // correct number of arguments
    if(strcmp(type, "x") != 0 && argc > 5){
        printf("No need for additional arguments, they will be ignored\n");
    }
    if(strcmp(type, "x") == 0 && argc < 6){
        printf("To show the sections, you need to add additional arguments\n\n");
        usage(argv[0]);
        exit(1);
    }

    Elf32_Ehdr* Header = malloc(sizeof(Elf32_Ehdr));
    Elf32_Shdr* SectionTable = malloc(sizeof(Elf32_Shdr));
    Elf32_Sym* symTab = malloc(sizeof(Elf32_Sym));
	Elf32_Rel* rel = malloc(sizeof(Elf32_Rel));
	Elf32_Rela* rela = malloc(sizeof(Elf32_Rela));
    // HEADER
    if(strcmp(type, "h") == 0){
        printf("\nAll values are in decimal format.\n");
        printf("All sizes are written in bytes.\n");
        printf("ELF Header Reader : \n\n");

        getHeader(file, Header, 1);
    }else{ // type != "h"
        getHeader(file, Header, 0);
        fseek(file, Header->e_shoff, SEEK_SET);
    }
    
    if(strcmp(type, "S") == 0){
        printf("\nAdresses  are given un hexadecimal format.\n");
        printf("All values are given in bytes in decimal format.\n\n");
        printf("Nb\tName\t\t\tType\t\tFlags\tExecutionAdresss\tOffset\t\tSectionSize\tLinkTo\tInfo\tAlign\tEntrySize\n");
        printf("====================================================================");
        printf("=============================================================================\n");
        
        if (Header->e_shnum == 0){
            printf("No section Table...\n");
        }else{
            for (int i=0; i<Header->e_shnum; i++){
                printNumber(Header, i);
                getSectionTable(file, Header, SectionTable, -1, 1);
                printf("\n");
            }
        }
    }else if(strcmp(type, "x") == 0){
        /* Pour vérifier si on cherche par Nom/Numéro, on strol(argv[2])
            Si cela échoue, endPointer est le même pointeur que argv[2], donc argv[2] est un char on fait une recherche par nom
            Si cela réussit, endPointer est toujours NULL donc argv[2] est un nombre on fait une recherche par numéro */
        char* endPointer = NULL;
        int sectionSelected = strtol(argv[6], &endPointer, 10);
        if (endPointer==argv[6]){

            /* Search by Name */
            printf("Search by Name\n");
            
            /* On vérifie qu'il y a une section avec le nom voulu */
            if (getSectionName(file, Header, SectionTable, argv[6], 0)){
                printf("Section Found!\n");
                printContent(file, SectionTable, -1, argv[6]);
            }else{
                printf("\nThere are no section called \"%s\".", argv[6]);
                exit(0);
            }

        }else{

            /* Search by Number */
            printf("Search by Number\n");
            if (sectionSelected > Header->e_shnum){
                printf("This section does not exist !\n");
                printf("There are only %d sections.\n", Header->e_shnum);
                exit(0);
            }

            getSectionTable(file, Header, SectionTable, sectionSelected, 0);

            if (SectionTable->sh_size == 0){
                /* Une section de taille de zéro n'a pas de data à afficher */
                printf("There is no data to dump.\n");
                exit(0);
            }

            /* On a recupéré la table qu'on voulait, on affiche son contenu*/
            printContent(file, SectionTable, sectionSelected, NULL);
	    }
    }else if(strcmp(type, "s") == 0){
        printf("\nAdresses are given un hexadecimal format.\n");
        printf("All values are given in bytes in decimal format.\n\n");
        printf("Nb\tName\t\tValue\t\tSize\tBind\tType\t\tOther\tShndx\n");
        printf("====================================================================");
        printf("=====================================================================\n");
	
	    GetTableSymbPart(file, Header, SectionTable, 0);
	    fseek(file, SectionTable->sh_offset, SEEK_SET);	
        for (int i = 0; i < SectionTable->sh_size/16; i++){
            printf("%d\t", i);
            getTabSymb(file, Header, symTab, 1);
            printf("\n");
        }
    }else if(strcmp(type, "r") == 0){
        printf("\nAdresses are given un hexadecimal format.\n");
        printf("All values are given in bytes in decimal format.\n\n");

        GetRelocationPart(file, Header,SectionTable, rel, rela, 0);
        printf("\n");
    }
	fclose(file);
	return 0;
}