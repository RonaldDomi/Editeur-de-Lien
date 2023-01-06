	//
	//  mergeelf.c
	//  Editeur-de-lien
	//
	//  Created by Bastien Levasseur on 03/01/2023.
	//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/CheckElf.h"
#include "modules/CustomElf.h"
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "modules/readContent.h"
#include "mergeelf.h"


int main(int argc, char **argv){

	if (argc<3){
		printf("Usage : ./mergeelf <File1> <File2>\n");
		exit(1);
	}

	FILE *file1 = fopen(argv[1],"rb");
	FILE *file2 = fopen(argv[2],"rb");
	FILE *file3 = fopen("resMerge","w+");

	if (file1==NULL){
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}
	if (file2==NULL){
		printf("ERROR : %s no such file.\n", argv[2]);
		exit(1);
	}

	if (!checkELF(file1) && !checkELF(file2)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	Elf32_Ehdr* Header1 = malloc(sizeof(Elf32_Ehdr));
	Elf32_Ehdr* Header2 = malloc(sizeof(Elf32_Ehdr));
	Elf32_Shdr* SectionTable1 = malloc(sizeof(Elf32_Shdr));
	Elf32_Shdr* SectionTable2 = malloc(sizeof(Elf32_Shdr));

	getHeader(file1, Header1, 0);
	getHeader(file2, Header2, 0);

	/* On initialise un tableau de flags pour chaque section de SectionTable2, si 1 alors la section a déjà été fusionnée avec
	 une section de la partie 1, on doit donc l'ignorer */
	int alreadyCopied[Header2->e_shnum];
	for (int i=0; i<Header2->e_shnum; i++){
		alreadyCopied[i]=0;
	}

	long offset = 0;
	int merged = 0;
	int copied = 0;

	int nbSections=Header1->e_shnum + Header2->e_shnum;
	printf("On a au pire %d sections\n", nbSections);

	/* On choisit de copier d'abord toute la SectionTable 1 et de fusionner quand on peut */
	for (int i=0; i < Header1->e_shnum ; i++){
		printf("===Section %d===\n", i);
		printf("Pointeur à l'offset %lx\n",offset);
		/* On récupère une section de la table des deux fichiers */
		if (i < Header1->e_shnum){
			getSectionTable(file1, Header1, SectionTable1, i, 0);
		}else{
			printf("On a atteint la fin de la section 1, il reste la section 2 à copier!\n");
		}
		if (i < Header2->e_shnum){
			getSectionTable(file2, Header2, SectionTable2, i, 0);
		}else{
			printf("On a atteint la fin de la section 2, il reste la section 1 à copier!\n");
		}

		/* Si les deux sections sont de type PROGBITS */
		if (SectionTable1->sh_type == SHT_PROGBITS && SectionTable2->sh_type == SHT_PROGBITS){
			/* Si les deux sections ont le même nom */
			if (! strcmp(SectionTable1->sh_charname, SectionTable2->sh_charname)){
				/* On choisit de concaténer la section2 à la section1 */
				printf("Merge, une section en moins\n");
				fwrite(getContent(file1, SectionTable1), SectionTable1->sh_size, 1, file3);
				fwrite(getContent(file2, SectionTable2), SectionTable2->sh_size, 1, file3);

				alreadyCopied[i]=1;

				offset += SectionTable1->sh_size + SectionTable2->sh_size;
				nbSections--;
				merged++;
			}
		}else{
		/* Sinon, on copie simplement la section de SectionTable1 */
			fwrite(getContent(file1, SectionTable1), SectionTable1->sh_size, 1, file3);
			offset += SectionTable1->sh_size;
			copied++;
		}

		printf("On a maintenant au pire %d sections\n", nbSections);
		printf("Section suivante...\n\n");
	}

	/* On copie maintenant la partie 2 et tout ce qui n'a pas été fusionné */
	printf("=====Section table 2=====\n\n");

	int sectionNumber=Header1->e_shnum;

	for (int i=0; i < Header2->e_shnum; i++){
		printf("===Section %d===\n", sectionNumber);
		printf("Pointeur à l'offset %lx\n",offset);
		if (! alreadyCopied[i]){
			getSectionTable(file2, Header2, SectionTable2, i, 0);
			fwrite(getContent(file2, SectionTable2), SectionTable2->sh_size, 1, file3);
			printf("Ce sera la section %d\n",sectionNumber);

			offset += SectionTable2->sh_size;
			copied++;
			sectionNumber++;
		}else{
			printf("Section déjà copiée, pass\n");
			/* Déjà copié, pass*/
		}
		printf("On a maintenant au pire %d sections\n", nbSections);
		printf("Section suivante...\n\n");
	}

	fclose(file1);
	fclose(file2);
	fclose(file3);
	printf("Au total %d merges\n", merged);
	printf("Plus %d copies\n", copied);
	printf("On a donc au total %d sections\n", nbSections);
	printf("End\n");
}
