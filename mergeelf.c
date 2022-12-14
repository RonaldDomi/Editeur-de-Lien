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
#include "modules/readHeader.h"
#include "modules/readSectionTable.h"
#include "create_elf.h"


void mergeHeader(Elf32_Ehdr * ELF1_Header, Elf32_Ehdr * ELF2_Header, Elf32_Ehdr * ELF3_Header){
    // ELF3_Header->e_ident = ELF1_Header->e_ident;
	memcpy(ELF3_Header->e_ident, ELF1_Header->e_ident, 16);
    if(ELF1_Header->e_type == ELF2_Header->e_type){
        ELF3_Header->e_type = ELF1_Header->e_type;
    }
    else{
		printf("ERREUR : Les Fichiers ne sont pas de même type !");
        exit(1);
    }

    if(ELF1_Header->e_machine == ELF2_Header->e_machine){
        ELF3_Header->e_machine = ELF1_Header->e_machine;
    }
    else{
         printf("ERREUR : Les Fichiers ne sont pas de même architecture !");
        exit(1);
    }

    if(ELF1_Header->e_version == ELF2_Header->e_version){
        ELF3_Header->e_version = ELF1_Header->e_version;
    }
    else{
         printf("ERREUR : Les Fichiers ne sont pas de même version !");
        exit(1);
    }

    if(ELF1_Header->e_flags == ELF2_Header->e_flags){
        ELF3_Header->e_flags = ELF1_Header->e_flags;
    }
    else{
         printf("ERREUR : Les Fichiers n'ont pas les mêmes flags !");
        exit(1);
    }

    if(ELF1_Header->e_ehsize == ELF2_Header->e_ehsize){
        ELF3_Header->e_ehsize = ELF1_Header->e_ehsize;
    }
    else{
         printf("ERREUR : Les Fichiers n'ont pas la même taille de header !");
        exit(1);
    }

    if(ELF1_Header->e_phoff == ELF2_Header->e_phoff){
        ELF3_Header->e_phoff = ELF1_Header->e_phoff;
    }
    else{
         printf("ERREUR : Header different (e_phoff)!");
        exit(1);
    }

    if(ELF1_Header->e_phentsize == ELF2_Header->e_phentsize){
        ELF3_Header->e_phentsize = ELF1_Header->e_phentsize;
    }
    else{
         printf("ERREUR : Header different !");
        exit(1);
    }

    if(ELF1_Header->e_phnum == ELF2_Header->e_phnum){
        ELF3_Header->e_phnum = ELF1_Header->e_phnum;
    }
    else{
         printf("ERREUR : Header different !");
        exit(1);
    }

    if(ELF1_Header->e_shentsize == ELF2_Header->e_shentsize){
        ELF3_Header->e_shentsize = ELF1_Header->e_shentsize;
    }
    else{
         printf("ERREUR : Header different !");
        exit(1);
    }

    //     typedef struct
    // {
    //   unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
    //  = Elf32_Half e_type;                /* Object file type */
    //  = Elf32_Half e_machine;             /* Architecture */
    //  = Elf32_Word e_version;             /* Object file version */
    //  1 Elf32_Addr e_entry;               /* Entry point virtual address */
    //  = Elf32_Off e_phoff;                /* Program header table file offset */
    // x  Elf32_Off e_shoff;                /* Section header table file offset */
    //  = Elf32_Word e_flags;               /* Processor-specific flags */
    //  = Elf32_Half e_ehsize;              /* ELF header size in bytes */
    //  = Elf32_Half e_phentsize;           /* Program header table entry size */
    //  = Elf32_Half e_phnum;               /* Program header table entry count */
    //  = Elf32_Half e_shentsize;           /* Section header table entry size */
    // x  Elf32_Half e_shnum;               /* Section header table entry count */
    // ~1 Elf32_Half e_shstrndx;            /* Section header string table index */
    // } Elf32_Ehdr;

    
    ELF3_Header->e_shstrndx = ELF1_Header->e_shstrndx;
    ELF3_Header->e_entry = ELF1_Header->e_entry;

}

int arround(int* offset){
	while (*offset % 4 != 0)
	{
		*offset += 1;
	}
	return *offset;
}

/*fonction pas encore testee*/
/*fonction qui s'utilise en l'appelant l'orsque on a une section SYMTAB
cette fonction fait le merge de la SYMTAB donc plus besoin de concatener les section*/
// on renvoie la taille
int mergeSymbol(FILE* file, Elf32_Ehdr *Header1, Elf32_Ehdr *Header2, Elf32_AllSec * AllSectionsTables1, Elf32_AllSec * AllSectionsTables2, Elf32_SecContent SectionContent1, Elf32_SecContent SectionContent2, Elf32_SecContent SectionContent3, int nbTable1, int nbTable2, int* offset){
	int *alreadyCheck = malloc(4 * nbTable2);
	for (int l = 0; l < nbTable2; l++) alreadyCheck[l] = 0;
	char* nom1 = malloc(50);
	char* nom2 = malloc(50);
	Elf32_Sym* SymbolTables1 = malloc(sizeof(Elf32_Sym));
	Elf32_Sym* SymbolTables2 = malloc(sizeof(Elf32_Sym));
	int cur = 0;

	for (int i = 0; i < nbTable1 ; i++)
	{
		GetTableSymbPart(SectionContent1, SymbolTables1, i*16);
		strcpy(nom1, getStringSymbol(SymbolTables1->st_name, Header1, AllSectionsTables1));
		/*cas symbol local*/
		if (((SymbolTables1->st_info) >> 4) != 1)
		{
			// fwrite(SectionContent1 + i*16, 16, 1, file);
			memcpy(SectionContent3 + cur, SectionContent1 + i*16, 16);
			*offset += 16;
			cur+=16;
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
								// printf("\n\n\nERREUR DEUX SYMBOL DEFINIE\n\n\n");
								return cur;
							}
							else
							{
								// printf("\non merge symbole1 : %d avec symbole2 : %d\n", i, j);
								// fwrite(SectionContent1 + i*16, 16, 1, file);
								memcpy(SectionContent3 + cur, SectionContent1 + i*16, 16);
								cur += 16;
								*offset += 16;
							}
						}
						else
						{
							// printf("\non merge symbole2 : %d avec symbole1 : %d\n", j, i);
							// fwrite(SectionContent2 + j*16, 16, 1, file);
							memcpy(SectionContent3 + cur, SectionContent2 + j*16, 16);
							cur += 16;
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
			// fwrite(SectionContent2 + i*16, 16, 1, file);
			memcpy(SectionContent3 + cur, SectionContent2 + i*16, 16);
			cur += 16;
			*offset += 16;
		}
		
	}
	return cur;
}

int main(int argc, char **argv){

	if (argc<3){
		// printf("Usage : ./mergeelf <File1> <File2>\n");
		exit(1);
	}

	FILE *file1 = fopen(argv[1],"rb");
	FILE *file2 = fopen(argv[2],"rb");
	FILE *file3 = fopen("resMerge","w+");

	if (file1==NULL){
		// printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}
	if (file2==NULL){
		// printf("ERROR : %s no such file.\n", argv[2]);
		exit(1);
	}

	if (!checkELF(file1) && !checkELF(file2)){
		// printf("Not a ELF file !\n");
		// printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}

	Elf32_Info * ELF1 = getAllInfo(file1);
	Elf32_Info * ELF2 = getAllInfo(file2);
	Elf32_Info * ELF3 = malloc(sizeof(Elf32_Info));
	ELF3->Header = malloc(sizeof(Elf32_Ehdr));
    mergeHeader(ELF1->Header, ELF2->Header, ELF3->Header);

	ELF3->AllSections = malloc(sizeof(Elf32_AllSec));
	ELF3->AllSymbol = malloc(sizeof(Elf32_Sym) * (ELF1->AllSymbol->st_size/16 + ELF2->AllSymbol->st_size/16));

	/* On initialise un tableau de flags pour chaque section de SectionTable2, si 1 alors la section a déjà été fusionnée avec
	 une section de la partie 1, on doit donc l'ignorer */
	int alreadyCopied[ELF2->Header->e_shnum];
	for (int i=0; i<ELF2->Header->e_shnum; i++){
		alreadyCopied[i]=0;
	}

	int offset = 52;
	int merged = 0;
	int copied = 0;
	int taille = 0;
	ELF3->AllSections->nbSections = 0;

	int nbSections=ELF1->Header->e_shnum + ELF2->Header->e_shnum;
	// printf("On a au pire %d sections\n", nbSections);

	// printAllSectionsTables(file1, ELF1->AllSections, ELF1->Header);
	/* On choisit de copier d'abord toute la SectionTable 1 et de fusionner quand on peut */
	for (int i=0; i < ELF1->Header->e_shnum ; i++){
		// printf("===Section %d===\n", i);
		// printf("Pointeur à l'offset %d\n", arround(&offset));
		ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections] = malloc(10000);
		if(ELF1->AllSections->TabAllSec[i]->sh_type == SHT_SYMTAB)
		{
			for (int k=0; k < ELF2->Header->e_shnum; k++){
				if(ELF2->AllSections->TabAllSec[k]->sh_type == SHT_SYMTAB){
					// printf("\n--------SYMBOL TABLE----------\n");
					/*on rempli la structure section*/
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections] = malloc(sizeof(Elf32_Shdr));
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addr = ELF1->AllSections->TabAllSec[i]->sh_addr;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addralign = ELF1->AllSections->TabAllSec[i]->sh_addralign;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_entsize = ELF1->AllSections->TabAllSec[i]->sh_entsize;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_flags = ELF1->AllSections->TabAllSec[i]->sh_flags;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_info = ELF1->AllSections->TabAllSec[i]->sh_info;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_link = ELF1->AllSections->TabAllSec[i]->sh_link;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_name = ELF1->AllSections->TabAllSec[i]->sh_name;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_offset = offset;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_type = ELF1->AllSections->TabAllSec[i]->sh_type;					
					/*------------------------------*/
					taille = mergeSymbol(file3,ELF1->Header, ELF2->Header, ELF1->AllSections, ELF2->AllSections, ELF1->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSecContent[k], ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections], ELF1->AllSections->TabAllSec[i]->sh_size/16, ELF2->AllSections->TabAllSec[k]->sh_size/16, &offset);

					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_size = taille;

					ELF3->AllSections->nbSections++;
					// printf("\n------------------------------\n");
					alreadyCopied[k]=1;
					nbSections--;
					merged++;
					// printf("Merge, une section en moins\n");
					// printf("Merge section %d avec section %d\n", i, k);
					// printf("On a maintenant au pire %d sections\n", nbSections);
					// printf("section taille = %d\n", ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_size);
					// printf("Section suivante...\n\n");
				}
			}
		}
		else
		{	
			char nom1[50];
			strcpy(nom1, getStringSection(/*file1, */ELF1->AllSections->TabAllSec[i]->sh_name, ELF1->Header, ELF1->AllSections));
			
			// char * nom1 = getString(file1, ELF1->AllSections->TabAllSec[i]->sh_name, ELF1->Header, ELF1->AllSections);

			// fwrite(ELF1->AllSections->TabAllSecContent[i], ELF1->AllSections->TabAllSec[i]->sh_size, 1, file3);
			// on stock dans la structure
			memcpy(ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections], ELF1->AllSections->TabAllSecContent[i], ELF1->AllSections->TabAllSec[i]->sh_size);
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections] = malloc(sizeof(Elf32_Shdr));
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addr = ELF1->AllSections->TabAllSec[i]->sh_addr;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addralign = ELF1->AllSections->TabAllSec[i]->sh_addralign;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_entsize = ELF1->AllSections->TabAllSec[i]->sh_entsize;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_flags = ELF1->AllSections->TabAllSec[i]->sh_flags;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_info = ELF1->AllSections->TabAllSec[i]->sh_info;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_link = ELF1->AllSections->TabAllSec[i]->sh_link;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_name = ELF1->AllSections->TabAllSec[i]->sh_name;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_offset = offset;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_size = ELF1->AllSections->TabAllSec[i]->sh_size;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_type = ELF1->AllSections->TabAllSec[i]->sh_type;
			// ------------------------------------------------------------------
			offset += ELF1->AllSections->TabAllSec[i]->sh_size;
			copied++;
			ELF3->AllSections->nbSections++;

			for (int j=0; j < ELF2->Header->e_shnum; j++){
				char nom2[50];
				strcpy(nom2, getStringSection(/*file2,*/ ELF2->AllSections->TabAllSec[j]->sh_name, ELF2->Header, ELF2->AllSections));

				// char * nom2 = getString(file2, ELF2->AllSections->TabAllSec[j]->sh_name, ELF2->Header, ELF2->AllSections);
				// // printf("Nom 1 : <%s> & Nom 2 : <%s>\n", nom1, nom2);
				
				/* Si les deux sections ont le même nom */
				if (!strcmp(nom1, nom2)){
					/* On choisit de concaténer la section 2 à la section 1 */
					// printf("Merge, une section en moins\n");
					// printf("Merge section %d avec section %d\n", i, j);
					// fwrite(ELF2->AllSections->TabAllSecContent[j], ELF2->AllSections->TabAllSec[j]->sh_size, 1, file3);
					// on decale le pointeur de ce qu' on a copier pour copier la suite
					// autrement dit on concatene
					memcpy(ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections - 1] + ELF1->AllSections->TabAllSec[i]->sh_size, ELF2->AllSections->TabAllSecContent[j], ELF2->AllSections->TabAllSec[j]->sh_size);
					/*--------------------------------------------*/
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1] = malloc(sizeof(Elf32_Shdr));
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_addr = ELF2->AllSections->TabAllSec[j]->sh_addr;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_addralign = ELF2->AllSections->TabAllSec[j]->sh_addralign;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_entsize = ELF2->AllSections->TabAllSec[j]->sh_entsize;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_flags = ELF2->AllSections->TabAllSec[j]->sh_flags;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_info = ELF2->AllSections->TabAllSec[j]->sh_info;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_link = ELF2->AllSections->TabAllSec[j]->sh_link;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_name = ELF2->AllSections->TabAllSec[j]->sh_name;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_offset = offset;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_size = ELF1->AllSections->TabAllSec[i]->sh_size + ELF2->AllSections->TabAllSec[j]->sh_size;
					ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_type = ELF2->AllSections->TabAllSec[j]->sh_type;
					/*--------------------------------------------*/

					alreadyCopied[j]=1;

					offset += ELF2->AllSections->TabAllSec[j]->sh_size;
					nbSections--;
					merged++;
					break;
				}
			}

			// printf("On a maintenant au pire %d sections\n", nbSections);
			// printf("section taille = %d\n", ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_size);
			// printf("Section suivante...\n\n");
		}
	}
	/* On copie maintenant la partie 2 et tout ce qui n'a pas été fusionné */
	// printf("=====Section table 2=====\n\n");

	int sectionNumber=ELF1->Header->e_shnum;

	// for (int i=0; i < ELF2->Header->e_shnum; i++){
	// 	// printf("===Section %d===\n", sectionNumber);
	// 	// printf("Pointeur à l'offset %d\n",offset);

		if (! alreadyCopied[0]){
		// 	// fwrite(ELF2->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSec[i]->sh_size, 1, file3);
		// 	// printf("Ce sera la section %d\n", sectionNumber);
		// 	// on stock dans la structure
		// 	memcpy(ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections - 1], ELF2->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSec[i]->sh_size);
        }
    for (int i=0; i < ELF2->Header->e_shnum; i++){
		// printf("===Section %d===\n", sectionNumber);
		// printf("Pointeur à l'offset %d\n",arround(&offset));
		ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections] = malloc(10000);
		if (! alreadyCopied[i]){
			// fwrite(ELF2->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSec[i]->sh_size, 1, file3);
			// printf("Ce sera la section %d\n", sectionNumber);
			// on stock dans la structure
			memcpy(ELF3->AllSections->TabAllSecContent[ELF3->AllSections->nbSections], ELF2->AllSections->TabAllSecContent[i], ELF2->AllSections->TabAllSec[i]->sh_size);
			offset += ELF2->AllSections->TabAllSec[i]->sh_size;
			/*--------------------------------------*/
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections] = malloc(sizeof(Elf32_Shdr));
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addr = ELF2->AllSections->TabAllSec[i]->sh_addr;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_addralign = ELF2->AllSections->TabAllSec[i]->sh_addralign;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_entsize = ELF2->AllSections->TabAllSec[i]->sh_entsize;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_flags = ELF2->AllSections->TabAllSec[i]->sh_flags;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_info = ELF2->AllSections->TabAllSec[i]->sh_info;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_link = ELF2->AllSections->TabAllSec[i]->sh_link;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_name = ELF2->AllSections->TabAllSec[i]->sh_name;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_offset = offset;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_size = ELF2->AllSections->TabAllSec[i]->sh_size;
			ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections]->sh_type = ELF2->AllSections->TabAllSec[i]->sh_type;
			/*--------------------------------------*/

			ELF3->AllSections->nbSections++;
			copied++;
			sectionNumber++;
		}else{
			// printf("Section déjà copiée, pass\n");
			/* Déjà copié, pass*/
		}
		// printf("section taille = %d\n", ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_size);
		// printf("Section suivante...\n\n");
	}
    ELF3->Header->e_shoff = offset + ELF3->AllSections->TabAllSec[ELF3->AllSections->nbSections-1]->sh_size;
    ELF3->Header->e_shoff = arround((int*)&ELF3->Header->e_shoff);
    ELF3->Header->e_shnum = ELF3->AllSections->nbSections;

    // printHeader(ELF3->Header);
	// printf("Last sec offset %d: ", ELF3->AllSections->TabAllSec[24]->sh_size);
	write_to_file(ELF3, file3);
	printHeader(ELF3->Header);
	printAllSectionsTables(file1, ELF3->AllSections, ELF3->Header);

	freeElf32_Inf(ELF1);
    freeElf32_Inf(ELF2);
    freeElf32_Inf(ELF3);
	
	fclose(file1);
	fclose(file2);
	fclose(file3);
	// printf("Au total %d merges\n", merged);
	// printf("Plus %d copies\n", copied);
	// printf("On a donc au total %d sections\n", nbSections);
	// printf("on a %d Sections\n", ELF3->AllSections->nbSections);
	printf("End\n");
}
