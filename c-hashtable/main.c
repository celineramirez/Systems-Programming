#include "headers.h"

#define INPUT_BUF_SIZE 60 // Size of the buffer used to read records from input file
#define    SEARCH_BUF_SIZE 25 // Size of the buffer used to read records from search file

struct student createStudent(char record[]);

void processInputFile(struct student *hashTable[], char *filename);

void processSearchFile(struct student *hashTable[], char *filename); // extra credit

bool testRecord(char record[]);

int main(int argc, char *argv[]) {
    // initialize hash table
    struct student *s[TABLE_SIZE] = {0};
    initializeTable(s);

    // File handling
    printf("Enter the file name:\n");
    char *file = argv[1];

    processInputFile(s, file);
    displayHashTable(s);
    return 0;
}

struct student createStudent(char record[]) {
    struct student s1;

    // Do not forget to use the following functions:
    // strtok() - splits a string into separate parts
    // strcpy() - copies string data from one variable to another
    // strtol() - converts a string to its integer value
    // atof() 	- converts a string to its double value

    // extract data from file and enter in student structure
    char *token = strtok(record, ",");
    strcpy(s1.name, token);
    token = strtok(NULL, ",");
    strcpy(s1.number, token);
    token = strtok(NULL, ",");
    strcpy(s1.email, token);
    token = strtok(NULL, ",");
    s1.credits = strtol(token, NULL, 10);
    token = strtok(NULL, ",");
    s1.gpa = atof(token);

    return s1;
}// end createStudent

void processInputFile(struct student *hashTable[], char *filename) {
    // Do not forget to use the following functions:
    // memset() - fill the buffer array with the specified character
    FILE *fp = fopen(filename, "r");

    struct student newStudent;

    // read the file, and send to createstudent line by line
    char buffer[INPUT_BUF_SIZE];

    if (fp == NULL) {
        displayError(FILE_NOT_FOUND, filename);
        exit(0);
    }
    printf("File opened successfully\n");

    while (fgets(buffer, INPUT_BUF_SIZE, fp) != NULL) {

        if (testRecord(buffer) == true) {
            newStudent = createStudent(buffer);
            if(testStudentData(newStudent) == true) {
                insertStudent(hashTable, &newStudent);
            }
            else {

            }
        }
        else {

        }

        memset(buffer, 0, INPUT_BUF_SIZE);

    }

    fclose(fp);

}// end processInputFile

void processSearchFile(struct student *hashTable[], char *filename) {
    // Do not forget to use the following functions:
    // strtok() - splits a string into separate parts
    // memset() - fill the buffer array with the specified character
}

bool testRecord(char record[]) {

    int count = 0;

    for (int i = 0; i < strlen(record); i++) {
        if (record[i] == ',') {
            count++;
        }
    }

    if (record[0] == 10) {
        printf("\nError: ");
        displayError(BLANK_RECORD, NULL);
        return false;
    } else if ((strlen(record) < 35) || (strlen(record) > 60)) {
        printf("\nError: ");
        displayError(SHORT_RECORD, NULL);
        printf("(%s)\n", record);
        return false;
    } else if (count != 4) {
        printf("\nError: ");
        displayError(CORRUPT_RECORD, NULL);
        printf("(%s)\n", record);
        return false;
    }

    return true;

}// end testRecord
