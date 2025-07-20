#include "headers.h"

// To implement Pass 2 of the assembler for Project 3,
// Add the UNDEFINED_SYMBOL error to the displayError() function
void displayError(int errorType, char* errorInfo)
{
    switch (errorType) {

        case MISSING_COMMAND_LINE_ARGUMENTS:
            printf("ERROR: Missing command line arguments ");
            break;

        case FILE_NOT_FOUND:
            printf("ERROR: File was not found ");
            break;

        case BLANK_RECORD:
            printf("ERROR: Source File Contains Blank Lines. ");
            break;

        case DUPLICATE:
            printf("ERROR: Duplicate Symbol Name (%s) Found in Source File. ", errorInfo);
            break;

        case ILLEGAL_OPCODE_DIRECTIVE:
            printf("ERROR: Illegal Opcode or Directive (%s) Found in Source File ", errorInfo);
            break;

        case ILLEGAL_SYMBOL:
            printf("ERROR: Symbol Name (%s) Cannot be a Command or Directive.", errorInfo);
            break;

        case OUT_OF_MEMORY:
            printf("ERROR: Program Address (0x%X) Exceeds Maximum Memory Address [0x8000]. ", errorInfo);
            break;

        case OUT_OF_RANGE_BYTE:
            printf("ERROR: Byte outside hex range of 00 to FF ");
            break;

        case OUT_OF_RANGE_WORD:
            printf("ERROR: Word outside decimal range of [-16,777,216 to 16,777,215] ");
            break;

        case UNDEFINED_SYMBOL:
            printf("Symbol not found in table ");
            break;

        default:
            break;
}