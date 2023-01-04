#include <stdio.h>
#include <stdlib.h>
// #include "modules/freadoctet.h"
#include "modules/CustomElf.h"
#include "modules/CheckElf.h"
#include "modules/readHeader.h"

int main(int argc, char **argv)
{

	/* Vérifie le nombre d'arguments */
	if (argc < 2)
	{
		printf("Usage : ./read_elf_header <FichierBinaire>\n");
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

	/* Alloue de la mémoire pour une en-tête et la remplit */
	Elf32_Ehdr *Header = malloc(sizeof(Elf32_Ehdr));

	/* On prévoit l'affichage pour l'étape 1 */
	printf("\nAll values are in decimal format.\n");
	printf("All sizes are written in bytes.\n");
	printf("ELF Header Reader : \n\n");

	getHeader(file, Header);
    printHeader(Header);

	fclose(file);
	return 0;
}
