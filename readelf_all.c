#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/readContent.h"
#include "modules/getTableSymbSection.h"
#include "modules/readRelocation.h"
#include "modules/readAll.h"


void usage(char *name)
{
	fprintf(stderr, "Usage:\n"
			"%s [ --help ] [ --file file ] [ --type h|S|x|s|r ] [ --arg number/string ] file\n\n"
			"Prints the elf structure of the file passed as first argument.\n ",
			name);
}

int main(int argc, char *argv[])
{
		// ###### Getting Arguments ######
		// printf("argc: %d\n", argc);
	if (argc < 5)
		{
		usage(argv[0]);
		exit(0);
		}
	int opt;
	char *filename, *type, *arg;

	struct option longopts[] = {
		{"filename", required_argument, NULL, '1'},
		{"type", required_argument, NULL, '2'},
		{"arg", required_argument, NULL, '3'},
		{"help", no_argument, NULL, 'h'},
		{NULL, 0, NULL, 0}};

	filename = NULL;
	type = NULL;
	arg = NULL;
	while ((opt = getopt_long(argc, argv, "1:2:3:h", longopts, NULL)) != -1)
		{
		switch (opt)
			{
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
	FILE *file = fopen(filename, "rb\n");
	if (file == NULL)
		{
		printf("ERROR : %s no such file.\n", filename);
		exit(1);
		}

	if (!checkELF(file))
		{
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
		}

		// ###### Starting readelf control ######
		// Correct type
	if (strcmp(type, "h") != 0 &&
		strcmp(type, "S") != 0 &&
		strcmp(type, "x") != 0 &&
		strcmp(type, "s") != 0 &&
		strcmp(type, "r") != 0)
		{
		printf("--type %s is not recognized, please retry with h|S|x|s|r \n", type);
		exit(0);
		}

		// Correct number of arguments
	if (strcmp(type, "x") != 0 && argc > 5)
		{
		printf("No need for additional arguments, they will be ignored\n");
		}
	if (strcmp(type, "x") == 0 && argc < 6)
		{
		printf("To show the sections, you need to add additional arguments\n\n");
		usage(argv[0]);
		exit(1);
		}


	Elf32_Info * ElfInfo = getAllInfo(file);
    // HEADER
	if (strcmp(type, "h") == 0)
		{
		printf("\nAll values are in decimal format.\n");
		printf("All sizes are written in bytes.\n");
		printf("ELF Header Reader : \n\n");

		printHeader(ElfInfo->Header);
		}

	if (strcmp(type, "S") == 0)
		{
		printAllTableSymb(ElfInfo->AllSymbol, ElfInfo->AllSections,ElfInfo->Header);
		}

	else if (strcmp(type, "x") == 0)
		{
		/* Pour vérifier si on cherche par Nom/Numéro, on strol(argv[6])
		 Si cela échoue, endPointer est le même pointeur que argv[6], donc argv[6] est un char on fait une recherche par nom
		 Si cela réussit, endPointer est toujours NULL donc argv[6] est un nombre on fait une recherche par numéro */
		char *endPointer = NULL;
		int sectionSelected = strtol(argv[6], &endPointer, 10);
		if (endPointer == argv[6])
			{
			/* Search by Name */
			printf("Search by Name\n");

			/* On vérifie qu'il y a une section avec le nom voulu */
			sectionSelected = getSectionByName(ElfInfo->AllSections, ElfInfo->Header, argv[6]);
			if (sectionSelected == -1)
				{
				printf("\nThere are no section called \"%s\".", argv[6]);
				exit(0);
				}
			}
		else
			{
			/* Search by Number */
			printf("Search by Number\n");
			if (sectionSelected > ElfInfo->Header->e_shnum)
				{
				printf("This section does not exist !\n");
				printf("There are only %d sections.\n", ElfInfo->Header->e_shnum);
				exit(0);
				}
			}
		printContent(ElfInfo->AllSections, sectionSelected);
		}
	else if (strcmp(type, "s") == 0)
		{
		printAllSectionsTables(file, ElfInfo->AllSections, ElfInfo->Header);
		}
	else if (strcmp(type, "r") == 0)
		{
        printRelocation(ElfInfo->AllSections, GetRelocationPart(file,  ElfInfo->Header, ElfInfo->AllSections), ElfInfo->Header);
		printf("\n");
		}

	if(arg != NULL)
		{
		;
		}

	fclose(file);
	return 0;
}
