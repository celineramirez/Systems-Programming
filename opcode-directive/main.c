#include "headers.h"

#define COMMENT 35
#define INPUT_BUF_SIZE 60
#define NEW_LINE 10
#define SPACE 32
#define SYMBOL_TABLE_SIZE 100

void performPass1(struct symbol *symbolArray[], char *filename, address *addresses);

struct segment *prepareSegments(char *line);

void trim(char string[]);

int main(int argc, char *argv[]) {
    // Do not modify this statement
    address addresses = {0x00, 0x00, 0x00};

    // Your code should start here

    // Create and initialize the symbol table
    struct symbol *sym[SYMBOL_TABLE_SIZE] = {0};
    initializeSymbolTable(sym);

    if (argc == 2) {
        performPass1(sym, argv[1], &addresses);
        displaySymbolTable(sym);
    } else {
        displayError(MISSING_COMMAND_LINE_ARGUMENTS, NULL);
        exit(0);
    }
}// end main

void performPass1(struct symbol *symbolTable[], char *filename, address *addresses) {
    segment *seg;
    int reserveMemory = 0;
    FILE *fp = fopen(filename, "r");
    // read and hold the contents of the file
    char buffer[INPUT_BUF_SIZE];

    // check if file exists
    if (fp == NULL) {
        displayError(FILE_NOT_FOUND, filename);
        exit(0);
    }
    printf("File opened successfully\n");

    while (fgets(buffer, INPUT_BUF_SIZE, fp) != NULL) {
        // split line into segments
        seg = prepareSegments(buffer);

        int addr = strtol(seg->third, NULL, 10);
        // check the PC address, cannot be > 8000
        if (addresses->current > 8000) {
            displayError(OUT_OF_MEMORY, 0);
            exit(0);
        }

        else {
            // check if the record is blank
            if (buffer[0] < 32) {
                displayError(BLANK_RECORD, NULL);
                exit(0);
            }
                // check if comment
            else if (buffer[0] == 35) {
                continue;
            }
                // line is not blank or comment
            else {
//            printf("%s \n", buffer);

                // test if first segment is an opcode or directive (should not be)
                if ((isDirective(seg->first) != 0) || (isOpcode(seg->first) == 1)) {
                    displayError(ILLEGAL_SYMBOL, seg->first);
                    exit(0);
                }
                    // continue if first segment is not opcode/directive
                else {
                    // check if second segment is opcode/ directive
                    if (isDirective(seg->second) != 0) {
                        int directiveNum = isDirective(seg->second);
                        if (isStartDirective(directiveNum) == 1) {
                            addr = strtol(seg->third, NULL, 16);
                            addresses->start = addr;
                            addresses->current = addr;
                        } else {
                            reserveMemory = getMemoryAmount(isDirective(seg->second), seg->third);
                            addresses->increment = reserveMemory;
                        }
                    }
                    else if (isOpcode(seg->second) == 1) {
                        reserveMemory = 3;
                        addresses->increment = reserveMemory;
                    }
                    else {
                        displayError(ILLEGAL_OPCODE_DIRECTIVE, seg->second);
                        exit(0);
                    }
                    // Test whether the first segment (symbol) contains a symbol (length > 0)
                    if (strlen(seg->first) != 0 && strcmp(seg->second,"START") != 0 ) {
                        insertSymbol(symbolTable, seg->first, addresses->current);
                    }
                    addresses->current += addresses->increment;
                }
            }
        }
        memset(buffer, 0, INPUT_BUF_SIZE);
    }
    printf("\nAssembly Summary\n");
    printf("--------------------\n");
    printf("   Starting address:  0x%X\n", addresses->start);
    printf("     Ending address:  0x%X\n", addresses->current);
    printf("Program size(bytes):    %d\n", addresses->current);
    fclose(fp);
}// end performPass1

// Do no modify any part of this function
segment *prepareSegments(char *statement) {
    struct segment *temp = malloc(sizeof(segment));
    strncpy(temp->first, statement, SEGMENT_SIZE - 1);
    strncpy(temp->second, statement + SEGMENT_SIZE - 1, SEGMENT_SIZE - 1);
    strncpy(temp->third, statement + (SEGMENT_SIZE - 1) * 2, SEGMENT_SIZE - 1);

    trim(temp->first);
    trim(temp->second);
    trim(temp->third);
    return temp;
}

// Do no modify any part of this function
void trim(char value[]) {
    for (int x = 0; x < SEGMENT_SIZE; x++) {
        if (value[x] == SPACE) {
            value[x] = '\0';
        }
    }
}
