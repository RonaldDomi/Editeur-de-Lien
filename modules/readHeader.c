//
//  readHeader.c
//  Editeur de Liens
//
//  Created by Bastien Levasseur on 20/12/2022.
//

#include "readHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"
#include "freadoctet.h"


void magicNumber(FILE *file, Elf32_Ehdr* Header, char verbose){
	//On lit 16 octets du fichier, on les affichent si verbose
	if (verbose){
		printf("Magic number : ");
		fread(Header->e_ident , 16, 1, file);
		printOctet(Header->e_ident, 16, 0);
		printf("\n");
	}else{
		fread(Header->e_ident , 16, 1, file);
	}
}


void objectType(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Type objet : \t\t\t\t");
		fread(&Header->e_type, 2, 1, file);
		printf("%d ", Header->e_type);
		switch(Header->e_type){
			case(ET_NONE):
				printf("No type");
				break;
			case(ET_REL):
				printf("Relocatable file");
				break;
			case(ET_EXEC):
				printf("Executable file");
				break;
			case(ET_DYN):
				printf("Position-Independent Executable file");
				break;
			case(ET_CORE):
				printf("Core file");
				break;
			default: //Les processor-specifics n'ont pas étés implémentés, on peut les ajouter si besoin.
				printf("===WARNING: Unable to determine object type !===");
				break;
		}
		printf("\n");
	}else{
		fread(&Header->e_type, 2, 1, file);
	}
}


void machineType(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Type machine : \t\t\t\t");
		fread(&Header->e_machine, 2, 1, file);
		printf("%d ", Header->e_machine);
		switch(Header->e_machine){
			case(EM_NONE):
				printf("No Machine");
				break;
			case(EM_ARM):
				printf("ARM");
				break;
			case(EM_X86_64):
			case(EM_386):
				printf("Intel 80386");
				break;
			default: //On peut rajouter encore des types si besoin.
				printf("===UNK===");
				break;
		}
		printf("\n");
	}else{
		fread(&Header->e_machine, 2, 1, file);
	}
}


void version(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Version : \t\t\t\t");
		fread(&Header->e_version, 4, 1, file);
		printf("%d ", Header->e_version);
		switch(Header->e_version){
			case(EV_NONE):
				printf("Invalid ELF Version");
				break;
			case(EV_CURRENT):
				printf("Current ELF Version");
				break;
			default: //Peut-être retirer le warning si des extensions sont à prévoir, et que donc la current version sera supérieure à 1
				printf("===WARNING: Unknown version !===");
		}
		printf("\n");
	}else{
		fread(&Header->e_version, 4, 1, file);
	}
}


void entry(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Entry point adress : \t\t\t");
		fread(&Header->e_entry, 4, 1, file);
		printAdress(&Header->e_entry, 4, 1);
		printf("\n");
	}else{
		fread(&Header->e_entry, 4, 1, file);
	}
}


void progHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Program header offset : \t\t");
		fread(&Header->e_phoff, 4, 1, file);
		printf("%d Bytes\n", Header->e_phoff);
	}else{
		fread(&Header->e_phoff, 4, 1, file);
	}
}


void sectHeaderOff(FILE *file, Elf32_Ehdr* Header, char verbose){
	if (verbose){
		printf("Section header offset : \t\t");
		fread(&Header->e_shoff, 4, 1, file);
		printf("%d Bytes\n", Header->e_shoff);
	}else{
		fread(&Header->e_shoff, 4, 1, file);
	}
}


void flags(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Processor flags : \t\t\t");
		fread(&Header->e_flags, 4, 1, file);
		printAdress(&Header->e_flags, 4, 1);
		printf("\n");
	}else{
		fread(&Header->e_flags, 4, 1, file);
	}
}


void headerSize(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Header size : \t\t\t\t");
		fread(&Header->e_ehsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_ehsize);
	}else{
		fread(&Header->e_ehsize, 2, 1, file);
	}
}


void headerProgramSize(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Program header size : \t\t\t");
		fread(&Header->e_phentsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_phentsize);
	}else{
		fread(&Header->e_phentsize, 2, 1, file);
	}

}


void nbProgramHeaders(FILE *file,Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Number of program headers : \t\t");
		fread(&Header->e_phnum, 2, 1, file);
		printf("%d Headers\n", Header->e_phnum);
	}else{
		fread(&Header->e_phnum, 2, 1, file);
	}
}


void sizeSectionHeaders(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Size of section headers : \t\t");
		fread(&Header->e_shentsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_shentsize);
	}else{
		fread(&Header->e_shentsize, 2, 1, file);
	}
}


void nbSectionHeader(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Number of section headers : \t\t");
		fread(&Header->e_shnum, 2, 1, file);
		printf("%d Headers\n", Header->e_shnum);
	}else{
		fread(&Header->e_shnum, 2, 1, file);
	}
}


void indexStringHeader(FILE *file, Elf32_Ehdr *Header, char verbose){
	if (verbose){
		printf("Index section string table : \t\t");
		fread(&Header->e_shstrndx, 2, 1, file);
		printf("%d\n", Header->e_shstrndx);
	}else{
		fread(&Header->e_shstrndx, 2, 1, file);
	}
}


void getHeader(FILE *file, Elf32_Ehdr *Header, char verbose){
	magicNumber(file, Header, verbose);
	objectType(file, Header, verbose);
	machineType(file, Header, verbose);
	version(file, Header, verbose);
	entry(file, Header, verbose);
	progHeaderOff(file, Header, verbose);
	sectHeaderOff(file, Header, verbose);
	flags(file, Header, verbose);
	headerSize(file, Header, verbose);
	headerProgramSize(file, Header, verbose);
	nbProgramHeaders(file, Header, verbose);
	sizeSectionHeaders(file, Header, verbose);
	nbSectionHeader(file, Header, verbose);
	indexStringHeader(file, Header, verbose);
}
