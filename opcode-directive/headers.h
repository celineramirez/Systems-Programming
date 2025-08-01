#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NAME_SIZE 7
#define SEGMENT_SIZE 9

enum errors {
	BLANK_RECORD = 1, DUPLICATE, FILE_NOT_FOUND, ILLEGAL_OPCODE_DIRECTIVE, ILLEGAL_SYMBOL, 
	MISSING_COMMAND_LINE_ARGUMENTS, OUT_OF_MEMORY, OUT_OF_RANGE_BYTE, OUT_OF_RANGE_WORD
};

typedef struct address
{
	int start;
	int current;
	int increment;
} address;

typedef struct segment
{
	char first[SEGMENT_SIZE];
	char second[SEGMENT_SIZE];
	char third[SEGMENT_SIZE];
} segment;

typedef struct symbol
{
	char name[NAME_SIZE];
	int address;
} symbol;

// directives.c functions
int getMemoryAmount(int directiveType, char* string);
int isDirective(char* string);
bool isStartDirective(int directiveType);

// errors.c functions
void displayError(int errorType, char* errorInfo);

// opcodes.c functions
int getOpcodeValue(char* opcode);
bool isOpcode(char* string);

// symbols.c functions
void displaySymbolTable(struct symbol* symbolTable[]);
void initializeSymbolTable(struct symbol* symbolTable[]);
void insertSymbol(struct symbol* symbolTable[], char symbolName[], int symbolAddress);
