#include "headers.h"

#define MOD_SIZE 10
#define SYMBOL_TABLE_SEGMENTS 10
#define SYMBOL_TABLE_SIZE 100

int computeHash(char* symbolName);

int computeHash(char* symbolName)
{
    int sum = 0;

    for (int i = 0; i < strlen(symbolName); i++) {
        sum += symbolName[i];
    }

    return (sum % MOD_SIZE * SYMBOL_TABLE_SEGMENTS) % (SYMBOL_TABLE_SIZE - 1);
}

void displaySymbolTable(struct symbol* symbolTable[])
{
    printf("\nSymbol Table Contents\n");
    printf("-------------------\n");
    printf("%5s %-20s %6s\n", "Index", "Name", "Address");
    printf("%5s %-20s %6s\n", "-----", "--------------------", "---------", "-----------------", "-------",
           "---");
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        if (symbolTable[i] != NULL) {
            printf("%5d %-20s 0x%X\n", i, symbolTable[i]->name, symbolTable[i]->address);
        }
    }
}

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
int getSymbolAddress(struct symbol* symbolArray[], char* string)
{
    int hashval = computeHash(string);

    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++){

        // symbol exists at target location
        if (strcmp(symbolArray[hashval]->name, string) == 0) {

            return symbolArray[hashval]->address;

        }

        // target location is null
        else if (symbolArray[hashval] == NULL) {
            break;
        }

        // move to next location in symbol table
        else {
            hashval++;
        }
    }

    // if target location is null or end of table
    return -1;
}

void initializeSymbolTable(struct symbol* symbolTable[])
{
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        symbolTable[i] = 0;
    }
}

void insertSymbol(struct symbol* symbolTable[], char symbolName[], int symbolAddress)
{
    struct symbol *myTemp;
    bool isNull = true;

    int hashVal = computeHash(symbolName);

    while (isNull) {
        if (symbolTable[hashVal] == NULL) {
            myTemp = malloc(sizeof(struct symbol));
            symbolTable[hashVal] = myTemp;
            strcpy(symbolTable[hashVal]->name, symbolName);
            symbolTable[hashVal]->address = symbolAddress;

//            printf("\nAdded %s to Symbol Table at %d\n", symbolName, hashVal);
            isNull = false;
        } else {
            if (strcmp(symbolTable[hashVal]->name,symbolName)==0) {
                displayError(DUPLICATE, symbolName);
                exit(0);
            }
            hashVal++;
        }
    }
}
