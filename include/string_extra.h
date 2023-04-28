#ifndef STRING_EXTRA_H
#define STRING_EXTRA_H

#include <string.h>

void str_replace(char *str, char *strToReplace, char *replaceWith) {
    size_t strToLength = strlen(strToReplace);
    size_t strWithLength = strlen(replaceWith);

    while (*(str) != '\0') {
        if (strncmp(str, strToReplace, strToLength) == 0) {
            for (int i = 0; i < strWithLength; i++)
                str[i] = replaceWith[i];
            
            str += strWithLength;
        }

        str++;
    }
}

#endif