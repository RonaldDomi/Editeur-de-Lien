	//
	//  readHeader.c
	//  Editeur de Liens
	//

#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"
#include "freadoctet.h"
#include "readHeader.h"


int unused; // Var non utilisée pour les warnings lors du make

void decodeHeaderFlags(Elf32_Ehdr *Header)
{
	if (Header->e_flags & EF_ARM_EABI_VER5)
		{
		printf(", EABI Version 5");
		}
	else if (Header->e_flags & EF_ARM_EABI_VER4)
		{
		printf(", EABI Version 4");
		}
	else if (Header->e_flags & EF_ARM_EABI_VER3)
		{
		printf(", EABI Version 3");
		}
	else if (Header->e_flags & EF_ARM_EABI_VER2)
		{
		printf(", EABI Version 2");
		}
	else if (Header->e_flags & EF_ARM_EABI_VER1)
		{
		printf(", EABI Version 1");
		}
	else if (Header->e_flags & EF_ARM_EABI_UNKNOWN)
		{
		printf(", EABI Unknown");
		}

	if (Header->e_flags & EF_ARM_RELEXEC)
		{
		printf(", Relexec");
		}
	else if (Header->e_flags & EF_ARM_HASENTRY)
		{
		printf(", Rasentry");
		}
	else if (Header->e_flags & EF_ARM_INTERWORK)
		{
		printf(", Interwork");
		}
	else if (Header->e_flags & EF_ARM_APCS_26)
		{
		printf(", APCS 26");
		}
	else if (Header->e_flags & EF_ARM_APCS_FLOAT)
		{
		printf(", APCS Float");
		}
	else if (Header->e_flags & EF_ARM_PIC)
		{
		printf(", PIC");
		}
	else if (Header->e_flags & EF_ARM_ALIGN8)
		{
		printf(", Align 8");
		}
	else if (Header->e_flags & EF_ARM_NEW_ABI)
		{
		printf(", new ABI");
		}
	else if (Header->e_flags & EF_ARM_OLD_ABI)
		{
		printf(", old ABI");
		}
	else if (Header->e_flags & EF_ARM_SOFT_FLOAT)
		{
		printf(", Soft Float");
		}
	else if (Header->e_flags & EF_ARM_VFP_FLOAT)
		{
		printf(", VFP Float");
		}
	else if (Header->e_flags & EF_ARM_MAVERICK_FLOAT)
		{
		printf(", Maverick Float");
	}else if (Header->e_flags == 0){
		printf(", ==NO_FLAGS==");
	}else{
		printf(", ==UNK== %02x", Header->e_flags);
	}
}

void printHeader(Elf32_Ehdr *Header){

	printf("Magic number : ");
	printOctet(Header->e_ident, 16, 0);
	printf("\n");


	printf("Type objet : \t\t\t\t");
	printf("%d ", Header->e_type);
	switch (Header->e_type)
		{
			case (ET_NONE):
			printf("No type");
			break;
			case (ET_REL):
			printf("Relocatable file");
			break;
			case (ET_EXEC):
			printf("Executable file");
			break;
			case (ET_DYN):
			printf("Position-Independent Executable file");
			break;
			case (ET_CORE):
			printf("Core file");
			break;
			default: // Les processor-specifics n'ont pas étés implémentés, on peut les ajouter si besoin.
			printf("===WARNING: Unable to determine object type !===");
			break;
		}
	printf("\n");


	printf("Type machine : \t\t\t\t");
	printf("%d ", Header->e_machine);
	switch (Header->e_machine)
		{
			case (EM_NONE):
			printf("No Machine");
			break;
			case (EM_ARM):
			printf("ARM");
			break;
			case (EM_X86_64):
			case (EM_386):
			printf("Intel 80386");
			break;
			default: // On peut rajouter encore des types si besoin.
			printf("===UNK===");
			break;
		}
	printf("\n");


	printf("Version : \t\t\t\t");
	printf("%d ", Header->e_version);
	switch (Header->e_version)
		{
			case (EV_NONE):
			printf("Invalid ELF Version");
			break;
			case (EV_CURRENT):
			printf("Current ELF Version");
			break;
			default: // Peut-être retirer le warning si des extensions sont à prévoir, et que donc la current version sera supérieure à 1
			printf("===WARNING: Unknown version !===");
		}
	printf("\n");


	printf("Entry point adress : \t\t\t");
	printAdress(&Header->e_entry, 4, 1);
	printf("\n");


	printf("Program header offset : \t\t");
	printf("%d Bytes\n", Header->e_phoff);


	printf("Section header offset : \t\t");
	printf("%d Bytes\n", Header->e_shoff);


	printf("Processor flags : \t\t\t");
	printAdress(&Header->e_flags, 4, 1);
	decodeHeaderFlags(Header);
	printf("\n");


	printf("Header size : \t\t\t\t");
	printf("%d Bytes\n", Header->e_ehsize);


	printf("Program header size : \t\t\t");
	printf("%d Bytes\n", Header->e_phentsize);


	printf("Number of program headers : \t\t");
	printf("%d Headers\n", Header->e_phnum);


	printf("Size of section headers : \t\t");
	printf("%d Bytes\n", Header->e_shentsize);


	printf("Number of section headers : \t\t");
	printf("%d Headers\n", Header->e_shnum);


	printf("Index section string table : \t\t");
	printf("%d\n", Header->e_shstrndx);
}

void getHeader(FILE *file, Elf32_Ehdr *Header)
{
	unused = fread(&Header->e_ident, 16, 1, file);
	unused = fread(&Header->e_type, 2, 1, file);
	unused = fread(&Header->e_machine, 2, 1, file);
	unused = fread(&Header->e_version, 4, 1, file);
	unused = fread(&Header->e_entry, 4, 1, file);
	unused = fread(&Header->e_phoff, 4, 1, file);
	unused = fread(&Header->e_shoff, 4, 1, file);
	unused = fread(&Header->e_flags, 4, 1, file);
	unused = fread(&Header->e_ehsize, 2, 1, file);
	unused = fread(&Header->e_phentsize, 2, 1, file);
	unused = fread(&Header->e_phnum, 2, 1, file);
	unused = fread(&Header->e_shentsize, 2, 1, file);
	unused = fread(&Header->e_shnum, 2, 1, file);
	unused = fread(&Header->e_shstrndx, 2, 1, file);
}
