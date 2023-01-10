#include <stdio.h>
#include <stdlib.h>

#include "lecture.h"
#include "util.h"

void lecture(Elf32_SecContent SectionContent, void* nbr, int length){
    for (int j = 0; j < length; j++)
    {
        ((char*)nbr)[j] = 0;
        ((char*)nbr)[j] = SectionContent[j];
    }
}

void lectureRev(Elf32_SecContent SectionContent, void* res, int length){
    int * buffer = malloc(length);
    for (int j = 0; j < length; j++)
    {
        ((char*)buffer)[j] = 0;
        ((char*)buffer)[j] = SectionContent[j];
    }

    if (length == 1){
        *(int *)res = *buffer;
    }
    else if (length == 2){
        *buffer = reverse_2(*buffer);
        *(int *)res = *buffer;
        // printf("chiffre2 : %x\n", *(int *)res);
    }
    else if (length == 4){
        *buffer = reverse_4(*buffer);
        *(int *)res = *buffer;
        // printf("chiffre2 : %x\n", *(int *)res);
    }
    free(buffer);
}