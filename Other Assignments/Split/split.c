#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "split.h"

char* join(char** a, int c) {

    size_t length = 0;
    //Determine length of string
    while (a[length] != 0)
    {
        length += strlen(a[length]);
    }

    //Initialize empty string
    char* str = malloc(length + 1);
    size_t index = 0;
    size_t placeInStr = 0;

    while (a[index] != 0) {

        for (size_t len = 0; len < strlen(a[index]); len++) {
            str[placeInStr] = a[index][len];
            placeInStr++;
        }

        //Inputs join char
        if (a[index + 1] != 0) {
            if (c != '\0') {
                str[placeInStr] = c;
                placeInStr++;
            }  
        }
        index += 1;
    }

    str[placeInStr] = '\0';

    return str;
}


char** split(const char* s, int c) {
    size_t rows = 0;
    size_t index = 0;

    //Figure out how many delimiters exist
    while (s[index] != '\0') {
        if (s[index] == c) {
            rows++;
        }
        index++;
    }

    char** strArray = malloc((rows + 2) * (sizeof(char*)) + 1);

    index = 0;
    size_t bigRow = 0;
    size_t beginSubstring = 0;
    size_t endSubstring = 0;

    while (s[index] != '\0')
    {

        if (s[index] == c) {
            endSubstring = index;

            //strArray[bigRow] = substring(s, beginSubstring, endSubstring);
            char* substring = malloc(endSubstring - beginSubstring);
            for (size_t locator = 0; locator < endSubstring - beginSubstring; locator++) {
                substring[locator] = s[beginSubstring + locator];
            }

            //Set string in strArray
            strArray[bigRow] = substring;
            bigRow++;

            //Set next begin
            beginSubstring = index + 1;
        }

        index++;

        if (s[index] == '\0') {
            endSubstring = index;

            if (endSubstring >= beginSubstring) {
                char* substring = malloc(endSubstring - beginSubstring);
                for (size_t locator = 0; locator < endSubstring - beginSubstring; locator++) {
                    substring[locator] = s[beginSubstring + locator];
                }

                //Set string in strArray
                strArray[bigRow] = substring;
                bigRow++;
            }
        }

    }

    return strArray;
}

void freeSplit(char** a) {
    size_t index = 0;
    while (a[index] != 0)
    {
        free(a[index]);
        index++;
    }
    free(a);
}
