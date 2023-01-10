#include "modules/CustomElf.h"

void write_to_file(Elf32_Info*, FILE*);
void write_header(Elf32_Ehdr*, FILE*);
void write_sections(Elf32_AllSec*, FILE*);
void write_section_header(Elf32_AllSec*, FILE*);