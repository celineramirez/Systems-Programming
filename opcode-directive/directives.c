#include "headers.h"

#define SINGLE_QUOTE 39

enum directives {
    ERROR, BYTE, END, RESB, RESW, START, WORD
};

int getMemoryAmount(int directiveType, char *string) {
    int numBytes;
    char hex[10];
    int stringVal = 0;
    int countStringBytes = 0;
    directiveType = isDirective(string);
    isStartDirective(directiveType);

    switch(directiveType){

        case BYTE :

        if (string[0] == 'X' && string[1] == SINGLE_QUOTE) {
            char hexnum[10];
            for(int i = 0; i < strlen(string); i++) {
                while (string[i] != SINGLE_QUOTE) {
                    hexnum[i-2] = string[i];
                    i++;
                }
            }

            int num = strtol(hexnum, NULL, 16);
            if(num < 0 || num > 255) {
                sprintf(hex, "%X", num);
                strtol(hex,NULL,16);
                displayError(OUT_OF_RANGE_BYTE, 0);
                exit(0);
            }

        } else if (string[0] == 'C') {
            countStringBytes = strlen(string) - 3;
            sprintf(hex,"%X", countStringBytes);
            return strtol(hex, NULL, 16);
        }
        return 1;

        case END:
            return 0;
        case START:
            return 0;
        case RESB:
        numBytes = strtol(string, NULL, 10);
        sprintf(hex, "%X", numBytes);
        return strtol(hex, NULL, 16);

        case RESW :
        numBytes = strtol(string, NULL, 10);
        sprintf(hex, "%X", numBytes);
        return 3 * strtol(hex, NULL, 16);
        case WORD :
        if ((atoi(string) < -16777216) || (atoi(string) > 16777215)) {
            displayError(OUT_OF_RANGE_WORD, string);
            exit(0);
        }
        default:
            sprintf(hex, "%X", 3);
            return strtol(hex,NULL, 16);
    }
}// end getMemoryAmount

int isDirective(char *string) {
    char *directives[] = {"ERROR", "BYTE", "END", "RESB", "RESW", "START", "WORD"};

    if (strcmp(directives[1], string) == 0) {
        return BYTE;
    }
    else if (strcmp(directives[2], string) == 0) {
        return END;
    }
    else if (strcmp(directives[3], string) == 0) {
        return RESB;
    }
    else if (strcmp(directives[4], string) == 0) {
        return RESW;
    }
    else if (strcmp(directives[5], string) == 0) {
        return START;
    }
    else if (strcmp(directives[6], string) == 0) {
        return WORD;
    }
    else {
        return ERROR;
    }
}

bool isStartDirective(int directiveType) {
    if (directiveType == START) {
        return true;
    } else {
        return false;
    }
}


