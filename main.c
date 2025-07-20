#include "headers.h"

#define COMMENT 35
#define INDEX_STRING ",X"
#define INPUT_BUF_SIZE 60
#define NEW_LINE 10
#define SPACE 32
#define SYMBOL_TABLE_SIZE 100

// Pass 2 constants
#define BLANK_INSTRUCTION 0x000000
#define OPCODE_MULTIPLIER 0x10000
#define OUTPUT_BUF_SIZE 70
#define RSUB_INSTRUCTION 0x4C0000
#define X_MULTIPLER 0x8000


void performPass1(struct symbol* symbolArray[], char* filename, address* addresses);
struct segment* prepareSegments(char* line);
void trim(char string[]);

// Pass 2 function prototypes
char* createFilename(char* filename, const char* extension);
void performPass2(struct symbol* symbolTable[], char* filename, address* addresses);
void resetObjectFileData(objectFileData* objectData, address* addresses);
void writeToLstFile(FILE* file, int address, segment* segments, int opcode);
void writeToObjFile(FILE* file, objectFileData data);

int main(int argc, char* argv[]) {
    address addresses = {0x00, 0x00, 0x00};

    // Your code should start here

    // Create and initialize the symbol table
    struct symbol *sym[SYMBOL_TABLE_SIZE] = {0};
    initializeSymbolTable(sym);

    if (argc == 2) {
        performPass1(sym, argv[1], &addresses);
        performPass2(sym, argv[1], &addresses);
//        displaySymbolTable(sym);
    } else {
        displayError(MISSING_COMMAND_LINE_ARGUMENTS, NULL);
        exit(0);
    }
}

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
char* createFilename(char* filename, const char* extension)
{
	char* temp = (char*)malloc(sizeof(char) * strlen(filename) + 1);
	
	strcpy(temp, filename);
	strcpy(temp, strtok(temp, "."));
	strcat(temp, extension);
	return temp;
}

void performPass1(struct symbol* symbolTable[], char* filename, address* addresses)
{
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
//    printf("\nAssembly Summary\n");
//    printf("--------------------\n");
//    printf("   Starting address:  0x%X\n", addresses->start);
//    printf("     Ending address:  0x%X\n", addresses->current);
//    printf("Program size(bytes):    %d\n", addresses->current);
    fclose(fp);
} // end perform pass1

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
void performPass2(struct symbol* symbolTable[], char* filename, address* addresses)
{
	// Do not modify this statement
	objectFileData objectData = { 0, { 0x0 }, { "\0" }, 0, 0x0, 0, { 0 }, 0, '\0', 0x0 };

	// Your code should start here
    segment *seg;
    recordEntry recEntry;
    int reserveMemory = 0;

    // open input file for reading
    FILE *fp = fopen(filename, "r");
    // read and hold the contents of the file
    char buffer[INPUT_BUF_SIZE];

    // check if file exists
    if (fp == NULL) {
        displayError(FILE_NOT_FOUND, filename);
        exit(0);
    }
    printf("File opened successfully\n");

    // create output files
    createFilename("sourceCode","lst");
    createFilename("objectCode","obj");

    // open output files for writing
    FILE *sc = fopen("sourceCode.lst", "w");
    FILE *oc = fopen("objectCode.obj", "w");

    while (fgets(buffer, INPUT_BUF_SIZE, fp) != NULL) {

        // check if the record is blank
        if (buffer[0] < 32) {
            displayError(BLANK_RECORD, NULL);
            exit(0);
        }
        // check if comment
        else if (buffer[0] == 35) {
            continue;
        }
        // line is neither blank nor comment
        else {
            // split line into segments
            seg = prepareSegments(buffer);

            // check if second segment is directive
            if(isDirective(seg->second) != 0){
                int directiveNum = isDirective(seg->second);
                if(isStartDirective(directiveNum) == true){

                    // assign rules to objectFileData structure
                    objectData.recordType ='H';
                    strcpy(objectData.programName, seg->first);
                    objectData.startAddress = addresses->start;
                    objectData.recordAddress = addresses->start;
                    objectData.programSize = addresses->current;

                    // set current address value to start
                    addresses->current = addresses->start;

                    // call obj and lst file
                    writeToObjFile(oc,objectData);
                    writeToLstFile(sc, addresses->current, seg, BLANK_INSTRUCTION);

                }
                else if (isEndDirective(directiveNum) == true) {
                    if(objectData.recordByteCount > 0) {
                        writeToObjFile(oc, objectData);
                        resetObjectFileData(&objectData, addresses);
                        objectData.recordType = 'E';
                        writeToObjFile(oc,objectData);
                        writeToLstFile(sc, addresses->current, seg, BLANK_INSTRUCTION);
                    }
                    else {
                        continue;
                    }
                }
                else if (isReserveDirective(directiveNum) == true) {
                    if(objectData.recordByteCount > 0) {
                        writeToObjFile(oc, objectData);
                        resetObjectFileData(&objectData, addresses);
                        writeToObjFile(oc,objectData);
                        writeToLstFile(sc, addresses->current, seg, BLANK_INSTRUCTION);
                        addresses->increment = getMemoryAmount(directiveNum,seg->second);
                        objectData.recordAddress += addresses->increment;
                    }
                    else {
                        continue;
                    }
                }
                else if(isDataDirective(directiveNum) == true){
                    addresses->increment = getMemoryAmount(directiveNum,seg->second);

                    // test if room in T record for BYTE/WORD value
                    if(objectData.recordByteCount > (MAX_RECORD_BYTE_COUNT - addresses->increment)){
                        writeToObjFile(oc, objectData);
                        resetObjectFileData(&objectData, addresses);
                        getByteWordValue(directiveNum, seg->second);

                        // add a record entry to objectFileData structure
                        recEntry.numBytes = addresses->increment;
                        recEntry.value = getByteWordValue(directiveNum, seg->second);

                        objectData.recordEntryCount += 1;
                        objectData.recordByteCount += addresses->increment;

                        writeToLstFile(sc, addresses->current, seg, recEntry.value);
                    }
                    else {
                        continue;
                    }

                }

            }// end check for directive

            // check if second segment is an opcode
            else if(isOpcode(seg->second) == true) {
                if(objectData.recordByteCount > (MAX_RECORD_BYTE_COUNT-3)){
                    writeToObjFile(oc, objectData);
                    resetObjectFileData(&objectData, addresses);
                    int opcodeval = getOpcodeValue(seg->second);
                    opcodeval *= OPCODE_MULTIPLIER;

                    // check if ",X" and get symbol address
                    if(strchr(seg->third, ',') != NULL && strchr(seg->third, 'X') != NULL){
                        opcodeval *= X_MULTIPLER;

                        // add third symbol without ,X
                        getSymbolAddress(symbolTable, seg->third);
                    }

                    // for opcodes that are not RSUB
                    if(strcmp(seg->second, "RSUB") != 0) {
                        int symbolAddress = getSymbolAddress(symbolTable, seg->third);
                        if (symbolAddress != -1) {
                            opcodeval += getSymbolAddress(symbolTable, seg->third);
                        }
                        else {
                            displayError(UNDEFINED_SYMBOL, seg->third);
                            exit(0);
                        }
                    }// end check if opcode is RSUB

                    recEntry.numBytes = 3;
                    recEntry.value = opcodeval;
                    objectData.recordEntryCount += 1;
                    objectData.recordByteCount += 3;

                    writeToLstFile(sc, addresses->current, seg, recEntry.value);
                    addresses->increment = 3;
                    addresses->current += addresses->increment;
                }// end if there is room in T record
            }// end check if opcode
        }// end else
        memset(buffer, 0, INPUT_BUF_SIZE);
    }// end while

    // close files
    fclose(fp);
    fclose(oc);
    fclose(sc);

}// end performPass2

segment* prepareSegments(char* statement)
{
	struct segment* temp = (segment*)malloc(sizeof(segment));
	strncpy(temp->first, statement, SEGMENT_SIZE - 1);
	strncpy(temp->second, statement + SEGMENT_SIZE - 1, SEGMENT_SIZE - 1);
	strncpy(temp->third, statement + (SEGMENT_SIZE - 1) * 2, SEGMENT_SIZE - 1);

	trim(temp->first);
	trim(temp->second);
	trim(temp->third);
	return temp;
}

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
void resetObjectFileData(objectFileData* objectData, address* addresses)
{
	objectData->recordAddress = addresses->current;
    objectData->recordByteCount = 0;
    objectData->recordEntryCount = 0;
}

// To implement Pass 2 of the assembler for Project 3,
// Use the following function to replace the function
// In your existing Project 2 code
void trim(char value[])
{
	for (int x = 0; x < SEGMENT_SIZE; x++)
	{
		if (value[x] == SPACE || x == (SEGMENT_SIZE - 1))
		{
			value[x] = '\0';
		}
	}
}

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
void writeToLstFile(FILE* file, int address, segment* segments, int opcode)
{
	if(isDirective(segments->second) != 0) {
        int directiveNum = isDirective(segments->second);
        if(isStartDirective(directiveNum) == true) {
            fprintf(file, "%08d %8s %8s %8s\n", address, segments->first, segments->second, segments->third);
        }
        else if (isEndDirective(directiveNum) == true){
            fprintf(file, "%08d %8s %8s %8s", address, segments->first, segments->second, segments->third);
        }
        else if (isReserveDirective(directiveNum) == true) {
            fprintf(file, "%08d %8s %8s %8s %06d\n", address, segments->first, segments->second, segments->third, opcode);
        }
        else if(isOpcode(segments->second) == true) {
            fprintf(file, "%08d %8s %8s %8s %06d\n", address, segments->first, segments->second, segments->third, opcode);
        }
    }
}

// To implement Pass 2 of the assembler for Project 3,
// Add the following function to your existing Project 2 code
void writeToObjFile(FILE* file, objectFileData fileData)
{
	if(fileData.recordType == 'H') {
        fprintf(file, "H %6s %06d %06d", fileData.programName, fileData.startAddress, fileData.programSize);
    }
    else if(fileData.recordType == 'T') {
        fprintf(file, "T %06d %06d", fileData.recordAddress, fileData.recordByteCount);
        // for each record entry
    }
    else if(fileData.recordType == 'E') {
        fprintf(file, "E %06d", fileData.startAddress);
    }

}