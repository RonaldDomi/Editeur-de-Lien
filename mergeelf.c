//
//  mergeelf.c
//  Editeur-de-lien
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modules/CheckElf.h"
#include "modules/readAll.h"
#include "modules/readStringTable.h"
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

	Elf32_Info * ELF1 = getAllInfo(file1);
	Elf32_Info * ELF2 = getAllInfo(file2);

	/* On initialise un tableau de flags pour chaque section de SectionTable2, si 1 alors la section a déjà été fusionnée avec
	 une section de la partie 1, on doit donc l'ignorer */
	int alreadyCopied[ELF2->Header->e_shnum];
	for (int i=0; i<ELF2->Header->e_shnum; i++){
		alreadyCopied[i]=0;
	}

	long offset = 0;
	int merged = 0;
	int copied = 0;

	int nbSections=ELF1->Header->e_shnum + ELF2->Header->e_shnum;
	printf("On a au pire %d sections\n", nbSections);

	// printAllSectionsTables(file1, ELF1->AllSections, ELF1->Header);
	/* On choisit de copier d'abord toute la SectionTable 1 et de fusionner quand on peut */
	for (int i=0; i < ELF1->Header->e_shnum ; i++){
		printf("===Section %d===\n", i);
		printf("Pointeur à l'offset %ld\n", offset);

		char nom1[50];
		strcpy(nom1, getStringSection(/*file1, */ELF1->AllSections->TabAllSec[i]->sh_name, ELF1->Header, ELF1->AllSections));
		
		// char * nom1 = getString(file1, ELF1->AllSections->TabAllSec[i]->sh_name, ELF1->Header, ELF1->AllSections);

		fwrite(ELF1->AllSections->TabAllSecContent[i], ELF1->AllSections->TabAllSec[i]->sh_size, 1, file3);
		offset += ELF1->AllSections->TabAllSec[i]->sh_size;
		copied++;

		for (int j=0; j < ELF2->Header->e_shnum; j++){

			char nom2[50];
			strcpy(nom2, getStringSection(/*file2,*/ ELF2->AllSections->TabAllSec[j]->sh_name, ELF2->Header, ELF2->AllSections));

			// char * nom2 = getString(file2, ELF2->AllSections->TabAllSec[j]->sh_name, ELF2->Header, ELF2->AllSections);
			// printf("Nom 1 : <%s> & Nom 2 : <%s>\n", nom1, nom2);
			
			/* Si les deux sections ont le même nom */
			if (! strcmp(nom1, nom2)){
				/* On choisit de concaténer la section 2 à la section 1 */
				printf("Merge, une section en moins\n");
				printf("Merge section %d avec section %d\n", i, j);
				fwrite(ELF2->AllSections->TabAllSecContent[j], ELF2->AllSections->TabAllSec[j]->sh_size, 1, file3);

				alreadyCopied[j]=1;

				offset += ELF2->AllSections->TabAllSec[j]->sh_size;
				nbSections--;
				merged++;
				break;
			}
		}

		printf("On a maintenant au pire %d sections\n", nbSections);
		printf("Section suivante...\n\n");
	}
	/* On copie maintenant la partie 2 et tout ce qui n'a pas été fusionné */
	printf("=====Section table 2=====\n\n");

	int sectionNumber=ELF1->Header->e_shnum;

	for (int i=0; i < ELF2->Header->e_shnum; i++){
		printf("===Section %d===\n", sectionNumber);
		printf("Pointeur à l'offset %lx\n",offset);

		if (! alreadyCopied[i]){
			fwrite(ELF2->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSec[i]->sh_size, 1, file3);
			printf("Ce sera la section %d\n", sectionNumber);

			offset += ELF2->AllSections->TabAllSec[i]->sh_size;
			copied++;
			sectionNumber++;
		}else{
			printf("Section déjà copiée, pass\n");
			/* Déjà copié, pass*/
		}
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
