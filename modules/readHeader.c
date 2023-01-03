//
//  readHeader.c
//  Editeur de Liens
//

#include "readHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include "CustomElf.h"
#include "freadoctet.h"

int unused; // Var non utilisée pour les warnings lors du make

void magicNumber(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	// On lit 16 octets du fichier, on les affichent si verbose
	if (verbose)
	{
		printf("Magic number : ");
		unused = fread(Header->e_ident, 16, 1, file);
		printOctet(Header->e_ident, 16, 0);
		printf("\n");
	}
	else
	{
		unused = fread(Header->e_ident, 16, 1, file);
	}
}

void objectType(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Type objet : \t\t\t\t");
		unused = fread(&Header->e_type, 2, 1, file);
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
	}
	else
	{
		unused = fread(&Header->e_type, 2, 1, file);
	}
}

void machineType(FILE *file, Elf32_Ehdr *Header, char verbose)
{

	if (verbose)
	{
		printf("Type machine : \t\t\t\t");
		unused = fread(&Header->e_machine, 2, 1, file);
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
	}
	else
	{
		unused = fread(&Header->e_machine, 2, 1, file);
	}
}

void version(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Version : \t\t\t\t");
		unused = fread(&Header->e_version, 4, 1, file);
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
	}
	else
	{
		unused = fread(&Header->e_version, 4, 1, file);
	}
}

void entry(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Entry point adress : \t\t\t");
		unused = fread(&Header->e_entry, 4, 1, file);
		printAdress(&Header->e_entry, 4, 1);
		printf("\n");
	}
	else
	{
		unused = fread(&Header->e_entry, 4, 1, file);
	}
}

void progHeaderOff(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Program header offset : \t\t");
		unused = fread(&Header->e_phoff, 4, 1, file);
		printf("%d Bytes\n", Header->e_phoff);
	}
	else
	{
		unused = fread(&Header->e_phoff, 4, 1, file);
	}
}

void sectHeaderOff(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Section header offset : \t\t");
		unused = fread(&Header->e_shoff, 4, 1, file);
		printf("%d Bytes\n", Header->e_shoff);
	}
	else
	{
		unused = fread(&Header->e_shoff, 4, 1, file);
	}
}

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
	}
}

void flags(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Processor flags : \t\t\t");
		unused = fread(&Header->e_flags, 4, 1, file);
		printAdress(&Header->e_flags, 4, 1);
		decodeHeaderFlags(Header);
		printf("\n");
	}
	else
	{
		unused = fread(&Header->e_flags, 4, 1, file);
	}
}

void headerSize(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Header size : \t\t\t\t");
		unused = fread(&Header->e_ehsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_ehsize);
	}
	else
	{
		unused = fread(&Header->e_ehsize, 2, 1, file);
	}
}

void headerProgramSize(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Program header size : \t\t\t");
		unused = fread(&Header->e_phentsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_phentsize);
	}
	else
	{
		unused = fread(&Header->e_phentsize, 2, 1, file);
	}
}

void nbProgramHeaders(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Number of program headers : \t\t");
		unused = fread(&Header->e_phnum, 2, 1, file);
		printf("%d Headers\n", Header->e_phnum);
	}
	else
	{
		unused = fread(&Header->e_phnum, 2, 1, file);
	}
}

void sizeSectionHeaders(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Size of section headers : \t\t");
		unused = fread(&Header->e_shentsize, 2, 1, file);
		printf("%d Bytes\n", Header->e_shentsize);
	}
	else
	{
		unused = fread(&Header->e_shentsize, 2, 1, file);
	}
}

void nbSectionHeader(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Number of section headers : \t\t");
		unused = fread(&Header->e_shnum, 2, 1, file);
		printf("%d Headers\n", Header->e_shnum);
	}
	else
	{
		unused = fread(&Header->e_shnum, 2, 1, file);
	}
}

void indexStringHeader(FILE *file, Elf32_Ehdr *Header, char verbose)
{
	if (verbose)
	{
		printf("Index section string table : \t\t");
		unused = fread(&Header->e_shstrndx, 2, 1, file);
		printf("%d\n", Header->e_shstrndx);
	}
	else
	{
		unused = fread(&Header->e_shstrndx, 2, 1, file);
	}
}

void getHeader(FILE *file, Elf32_Ehdr *Header, char verbose)
{
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
