#include <stdio.h>
#include <stdlib.h>
#include "modules/freadoctet.h"
#include "modules/CustomElf.h"


int checkELF(FILE *file){
	char buffer[4];
	fread(buffer, 4, 1, file);
	if (feof(file)){
		return 0;
	}
	if (buffer[0] != 0x7F){
		return 0;
	}
	if (buffer[1] != 0x45){
		return 0;
	}
	if (buffer[2] != 0x4C){
		return 0;
	}
	if (buffer[3] != 0x46){
		return 0;
	}
	return 1;
}


void magicNumber(FILE *file, Elf32_Ehdr* Header, char verbose){
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
				printf(" No type");
				break;
			case(ET_REL):
				printf(" Relocatable file");
				break;
			case(ET_EXEC):
				printf(" Executable file");
				break;
			case(ET_DYN):
				printf(" Shared object file");
				break;
			case(ET_CORE):
				printf(" Core file");
				break;
			default:
				printf(" ===WARNING: Unable to determine object type !===");
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
				printf(" No Machine");
				break;
			case(EM_ARM):
				printf(" ARM");
				break;
			case(EM_X86_64):
				printf(" AMD x86-64");
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
				printf(" Invalid ELF Version");
				break;
			case(EV_CURRENT):
				printf(" Current ELF Version");
				break;
			default:
				printf(" ===WARNING: Unknown version !===");
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
		printOctet(&Header->e_entry, 4, 1);
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
		printOctet(&Header->e_flags, 4, 1);
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
		printf("%d Bytes\n", Header->e_phentsize);
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


int main(int argc, char **argv){

	if (argc<2){
		printf("Usage : ./read_elf_header <FichierBinaire>\n");
		exit(1);
	}

	FILE *file = fopen(argv[1],"rb");
	if (file==NULL){
		printf("ERROR : %s no such file.\n", argv[1]);
		exit(1);
	}

	if (!checkELF(file)){
		printf("Not a ELF file !\n");
		printf("Does not have magic bytes 0x7F454C46 at the start.\n");
		exit(1);
	}
	fseek(file, -4, SEEK_CUR); /* Go backwards 4 bytes */

	Elf32_Ehdr* Header = malloc(sizeof(Elf32_Ehdr));

	printf("\nAll values are in decimal format.\n");
	printf("All sizes are written in bytes.\n");
	printf("ELF Header Reader : \n\n");

	getHeader(file, Header, 1);
	
	fclose(file);
	return 0;
}
