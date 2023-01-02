//
//  readSectionTable.c
//  Editeur de Liens
//
#include <math.h>
#include <string.h>
#include "readSectionTable.h"
#include "readStringTable.c"
#include "readHeader.h"
#include "CustomElf.h"
#include "freadoctet.h"
#include <string.h>
#include <math.h>

void sectionName(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char verbose, char nom){
	fread(&SectionTable->sh_name, 4, 1, file);
	if(nom){
		char* mot = malloc(50);
		long position = ftell(file);
		getString(file, SectionTable->sh_name, Header, mot);
		fseek(file, position, 0);
		SectionTable->sh_charname=mot;
		if (verbose){
			SectionTable->sh_charname="Temp";
			int wordLength = strlen(mot);
			if (wordLength==0){
				printf("==NO_NAME==\t\t");
			}else if ((0<=wordLength) && (8>wordLength)){
				printf("%s\t\t\t", mot);
			}else if ((8<=wordLength) && (16>wordLength)){
				printf("%s\t\t", mot);
			}else{
				printf("%s\t", mot);
			}
		}
		free(mot);
	}
}


void sectionType(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_type, 4, 1, file);
		switch(SectionTable->sh_type){ //Beaucoup de types à cause de firmware.elf pour éviter des =UNK=
			case(SHT_NULL):
				printf("NULL\t\t");
				break;
			case(SHT_PROGBITS):
				printf("PROGBITS\t");
				break;
			case(SHT_SYMTAB):
				printf("SYMTAB\t\t");
				break;
			case(SHT_STRTAB):
				printf("STRTAB\t\t");
				break;
			case(SHT_RELA):
				printf("RELA\t\t");
				break;
			case(SHT_HASH):
				printf("HASH\t\t");
				break;
			case(SHT_DYNAMIC):
				printf("DYNAMIC\t\t");
				break;
			case(SHT_NOTE):
				printf("NOTE\t\t");
				break;
			case(SHT_NOBITS):
				printf("NOBITS\t\t");
				break;
			case(SHT_REL):
				printf("REL\t\t");
				break;
			case(SHT_SHLIB):
				printf("SHLIB\t\t");
				break;
			case(SHT_LOPROC):
				printf("LOWPROC\t");
				break;
			case(SHT_HIPROC):
				printf("HIPROC\t\t");
				break;
			case(SHT_LOUSER):
				printf("LOWUSR\t\t");
				break;
			case(SHT_HIUSER):
				printf("HIUSR\t\t");
				break;
			case(SHT_PREINIT_ARRAY):
				printf("PREINIT_ARRAY\t");
				break;
			case(SHT_INIT_ARRAY):
				printf("INIT_ARRAY\t");
				break;
			case(SHT_FINI_ARRAY):
				printf("FINI_ARRAY\t");
				break;
			case(SHT_ARM_ATTRIBUTES):
				printf("ARM_ATTRIBUTES\t");
				break;
			case(SHT_ARM_EXIDX):
				printf("ARM_EXIDX\t");
				break;
			case(SHT_LOOS):
				printf("LOOS");
				break;
			default:
				printf("=UNK=\t\t");
				break;
		}
	}else{
		fread(&SectionTable->sh_type, 4, 1, file);
	}
}

void sectionFlags(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_flags, 4, 1, file);
		printf("%d ", SectionTable->sh_flags);
		printf("\t");
		//TODO: Décoder sh_flags en attributs (A, B, C ...) avec la Figure 1-11 page 13
	}else{
		fread(&SectionTable->sh_flags, 4, 1, file);
	}
}


void sectionAdress(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_addr, 4, 1, file);
		printAdress8(&SectionTable->sh_addr, 4, 1);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_addr, 4, 1, file);
	}
}


void sectionOffset(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_offset, 4, 1, file);
		printf("%d ", SectionTable->sh_offset);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_offset, 4, 1, file);
	}
}


void sectionSize(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_size, 4, 1, file);
		printf("%d ", SectionTable->sh_size);
		printf("\t\t");
	}else{
		fread(&SectionTable->sh_size, 4, 1, file);
	}
}


void sectionLink(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_link, 4, 1, file);
		printf("%d ",SectionTable->sh_link);
		printf("\t");
	}else{
		fread(&SectionTable->sh_link, 4, 1, file);
	}
}


void sectionInfo(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_info, 4, 1, file);
		printf("%d ",SectionTable->sh_info);
		printf("\t");
	}else{
		fread(&SectionTable->sh_info, 4, 1, file);
	}
}


void sectionAdressAlign(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_addralign, 4, 1, file);
		printf("%d ", SectionTable->sh_addralign);
		printf("\t");
	}else{
		fread(&SectionTable->sh_addralign, 4, 1, file);
	}
}


void sectionEntrySize(FILE *file, Elf32_Shdr* SectionTable, int verbose){
	if (verbose){
		fread(&SectionTable->sh_entsize, 4, 1, file);
		printf("%d ", SectionTable->sh_entsize);
		printf("\t");
	}else{
		fread(&SectionTable->sh_entsize, 4, 1, file);
	}
}


void printNumber(Elf32_Ehdr* Header, int sectionNumber){
	/* On calcule le nombre de caractères à imprimer entre les crochets,
	 cela revient à savoir combien de caractères on a besoin pour afficher
	 tous les numéros de table.
	 On utilise le log10()+1 pour calculer le nombre de caractères, on prend le floor() */

	/* Cas particulier si e_shnum=1, car log10(1-1) = -infinity */
	int charactersToPrint;
	if (Header->e_shnum==1){
		charactersToPrint = 1;
	}else{
		charactersToPrint = floor(log10(Header->e_shnum-1) +1);
		/*-1 car on part de 0, e.g: pour imprimer 10 nombres on a besoin que d'un caractère de 0 à 9*/
	}

	/* On cherche maintenant à savoir combien d'espaces on doit afficher avant le nombre
		On calcule la taille en chiffres du nombre de la section et on fait la soustraction */
	/* Cas particulier si la SectionNumber=0 ou 1 pour éviter -infinity */
	int spacesToPrint;
	if (sectionNumber==0 || sectionNumber==1){
		spacesToPrint = charactersToPrint - 1;
	}else{
		spacesToPrint = charactersToPrint - floor(log10(sectionNumber) +1);
	}

	printf("[");
	/* Cas particulier si on a un seul caractère à imprimer (donc 1 seul [0-9]), on imprime quand même un espace */
	/* Ce dernier cas rend toute la méthode inconsistante mais l'affichage de readelf -H est fait ainsi... */
	if (charactersToPrint == 1){
		printf(" ");
		printf("%d]\t", sectionNumber);
	}else{
		for (int i=0; i<spacesToPrint; i++){
			printf(" ");
		}
		printf("%d]\t", sectionNumber);
	}
}

void getSectionTable(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, int sectionNumber, int verbose){
	if (sectionNumber != -1){
		fseek(file, Header->e_shoff + (Header->e_shentsize * sectionNumber), SEEK_SET);
	}
	sectionName(file, Header, SectionTable, verbose, 1);
	sectionType(file, SectionTable, verbose);
	sectionFlags(file, SectionTable, verbose);
	sectionAdress(file, SectionTable, verbose);
	sectionOffset(file, SectionTable, verbose);
	sectionSize(file, SectionTable, verbose);
	sectionLink(file, SectionTable, verbose);
	sectionInfo(file, SectionTable, verbose);
	sectionAdressAlign(file, SectionTable, verbose);
	sectionEntrySize(file, SectionTable, verbose);
}


int getSectionName(FILE *file, Elf32_Ehdr* Header, Elf32_Shdr* SectionTable, char * sectionName, int verbose){
	for (int i=0; i<Header->e_shnum; i++){
		/*TODO: Il faudrait pouvoir mettre verbose=0, mais une seg fault se produit...*/
		getSectionTable(file, Header, SectionTable, i, 1);
//		printf("sh_charname : %s\n",SectionTable->sh_charname);
//		printf("Section searched : <%s>\n", sectionName);
		if (! strcmp(SectionTable->sh_charname, sectionName)){
			return 1;
		}
	}
	return 0;
}
