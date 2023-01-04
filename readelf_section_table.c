//
//  read_elf_section_table.c
//  Editeur de Liens
//

#include <stdio.h>
#include <stdlib.h>
#include "modules/CheckElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"

int main(int argc, char **argv)
{

	/* Vérifie le nombre d'arguments */
	if (argc < 2)
	{
		printf("Usage : ./read_elf_section_table <File>\n");
		exit(1);
	}

	/* Tente d'ouvrir un fichier avec son nom donné en argument */
	FILE *file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	/* Vérifie que le fichier est un ELF, renvoie une exception sinon*/
	if (!checkELF(file))
	{
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	/* Alloue de la mémoire pour une en-tête et la remplit
		Alloue aussi de la mémoire pour une en-tête de section */
	Elf32_Ehdr *Header = malloc(sizeof(Elf32_Ehdr));

	getHeader(file, Header);

	/* On va directement à l'adresse ou est contenue l'en-tête de la section et on la remplit
		On affiche aussi un début de tableau pour l'affichage de l'étape 2 */
	fseek(file, Header->e_shoff, SEEK_SET);
	printf("\n");
	printf("Adresses are given un hexadecimal format.\n");
	printf("All values are given in bytes in decimal format.\n\n");
	printf("Nb\tName\t\t\tType\t\tFlags\tExecutionAdresss\tOffset\t\tSectionSize\tLinkTo\tInfo\tAlign\tEntrySize\n");
	printf("====================================================================");
	printf("=============================================================================\n");

	if (Header->e_shnum == 0)
	{
		printf("No section Table...\n");
	}
	else
	{
        Elf32_AllSec * AllSectionTable = initSectionTable(Header->e_shnum);
		getAllSectionsTables(file, Header, AllSectionTable);
        printAllSectionsTables(AllSectionTable);
	}

	return 0;
}
