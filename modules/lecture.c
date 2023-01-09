#include <stdio.h>
#include <stdlib.h>

#include "lecture.h"

void lecture(Elf32_SecContent SectionContent, void* nbr, int length){
    for (int j = 0; j < length; j++)
    {
        ((char*)nbr)[j] = 0;
        ((char*)nbr)[j] = SectionContent[j];
    }
}