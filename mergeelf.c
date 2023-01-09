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
#include "modules/getTableSymbSection.h"

/*fonction pas encore testee*/
/*fonction qui s'utilise en l'appelant l'orsque on a une section SYMTAB
cette fonction fait le merge de la SYMTAB donc plus besoin de concatener les section*/
void mergeSymbol(FILE* file, Elf32_Ehdr *Header1, Elf32_Ehdr *Header2, Elf32_AllSec * AllSectionsTables1, Elf32_AllSec * AllSectionsTables2, Elf32_SecContent SectionContent1, Elf32_SecContent SectionContent2, int nbTable1, int nbTable2, int* offset){
	int *alreadyCheck = malloc(4 * nbTable2);
	for (int l = 0; l < nbTable2; l++) alreadyCheck[l] = 0;
	char* nom1 = malloc(50);
	char* nom2 = malloc(50);
	Elf32_Sym* SymbolTables1 = malloc(sizeof(Elf32_Sym));
	Elf32_Sym* SymbolTables2 = malloc(sizeof(Elf32_Sym));
	for (int i = 0; i < nbTable1 ; i++)
	{
		GetTableSymbPart(SectionContent1, SymbolTables1, i*16);
		strcpy(nom1, getStringSymbol(SymbolTables1->st_name, Header1, AllSectionsTables1));
		/*cas symbol local*/
		if (((SymbolTables1->st_info) >> 4) != 1)
		{
			fwrite(SectionContent1 + i*16, 16, 1, file);
			*offset += 16; 
		}
		/*cas symbol global*/
		else
		{
			for (int j = 0; j < nbTable2; j++)
			{
				
				GetTableSymbPart(SectionContent2, SymbolTables2, j*16);
				strcpy(nom2, getStringSymbol(SymbolTables2->st_name, Header2, AllSectionsTables2));
				// on verifie que les deux symbole ait le meme nom et qu il soit tout les deux global
				if (!strcmp(nom1, nom2) && ((SymbolTables1->st_info) >> 4) == 1 && ((SymbolTables2->st_info) >> 4) == 1)
				{
					/*on verifie que ca ne soit pas hidden*/
					if(SymbolTables1->st_other != 2)
					{	
						alreadyCheck[j] = 1;
						/*ici il faut ecrire le symbole definie*/
						if (SymbolTables1->st_shndx!=0){
							/*ici on verifie le quels est indefini*/
							if(SymbolTables2->st_shndx!=0){
								printf("\n\n\nERREUR DEUX SYMBOL DEFINIE\n\n\n");
								return;
							}
							else
							{
								printf("\non merge symbole1 : %d avec symbole2 : %d\n", i, j);
								fwrite(SectionContent1 + i*16, 16, 1, file);
								*offset += 16;
							}
						}
						else
						{
							printf("\non merge symbole2 : %d avec symbole1 : %d\n", j, i);
							fwrite(SectionContent2 + j*16, 16, 1, file);
							*offset += 16;
						}
					}
				}
			}
		}
	}
	/*on imprime la partie 2 qui na pas etait merge*/
	for (int i = 0; i < nbTable2; i++)
	{								
		GetTableSymbPart(SectionContent2, SymbolTables2, i*16);
									/*soit c'est globale soit ca a un nom*/
		if (!alreadyCheck[i] && (((SymbolTables2->st_info) >> 4) == 1 || SymbolTables2 ->st_name != 0))
		{
			fwrite(SectionContent2 + i*16, 16, 1, file);
			*offset += 16;
		}
		
	}
}

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

	int offset = 0;
	int merged = 0;
	int copied = 0;

	int nbSections=ELF1->Header->e_shnum + ELF2->Header->e_shnum;
	printf("On a au pire %d sections\n", nbSections);

	// printAllSectionsTables(file1, ELF1->AllSections, ELF1->Header);
	/* On choisit de copier d'abord toute la SectionTable 1 et de fusionner quand on peut */
	for (int i=0; i < ELF1->Header->e_shnum ; i++){
		printf("===Section %d===\n", i);
		printf("Pointeur à l'offset %d\n", offset);
		if(ELF1->AllSections->TabAllSec[i]->sh_type == SHT_SYMTAB)
		{
			for (int k=0; k < ELF2->Header->e_shnum; k++){
				if(ELF2->AllSections->TabAllSec[k]->sh_type == SHT_SYMTAB){
					printf("\n--------SYMBOL TABLE----------\n");
					mergeSymbol(file3,ELF1->Header, ELF2->Header, ELF1->AllSections, ELF2->AllSections, ELF1->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSecContent[k], ELF1->AllSections->TabAllSec[i]->sh_size/16, ELF2->AllSections->TabAllSec[k]->sh_size/16, &offset);
					printf("\n------------------------------\n");
					alreadyCopied[k]=1;
					nbSections--;
					merged++;
					printf("Merge, une section en moins\n");
					printf("Merge section %d avec section %d\n", i, k);
					printf("On a maintenant au pire %d sections\n", nbSections);
					printf("Section suivante...\n\n");
				}
			}
		}
		else
		{	
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
	}
	/* On copie maintenant la partie 2 et tout ce qui n'a pas été fusionné */
	printf("=====Section table 2=====\n\n");

	int sectionNumber=ELF1->Header->e_shnum;

	for (int i=0; i < ELF2->Header->e_shnum; i++){
		printf("===Section %d===\n", sectionNumber);
		printf("Pointeur à l'offset %d\n",offset);

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
