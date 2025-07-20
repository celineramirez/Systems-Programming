#pragma once // Prevents the header file from loading multiple times

// Constants for the Student Structure
#define EMAIL_SIZE 18
#define NAME_SIZE 24
#define NUMBER_SIZE 10

// Constants for the Hash Table
#define HASH_TABLE_SEGMENTS 7 // Used in the hashing algoritm
#define MOD_SIZE 5 // Used in the hashing algoritm
#define NEW_LINE 10 // Newline character code
#define TABLE_SIZE 35 // Number of available spaces

struct student {
    char name[NAME_SIZE];
    char number[NUMBER_SIZE];
    char email[EMAIL_SIZE];
    int credits;
    double gpa;
};

int computerHash(char *studentName);

void displayHashTable(struct student *hashTable[]);

void displayRecord(struct student *temp, int index);

void initializeTable(struct student *hashTable[]);

void insertStudent(struct student *hashTable[], struct student *temp);

bool testEmailFormat(struct student temp);

bool testStudentData(struct student temp);

int computerHash(char *input) {
    int sum = 0;

    for (int i = 0; i < strlen(input); i++) {
        sum += input[i];
    }

    return (sum % MOD_SIZE * HASH_TABLE_SEGMENTS) % (TABLE_SIZE - 1);
}

void displayHashTable(struct student *hashTable[]) {
    printf("\nHash Table Contents\n");
    printf("-------------------\n");
    printf("%5s %-20s %6s %17s %11s %5s\n", "Index", "Name", "Number", "Email Address", "Credits", "GPA");
    printf("%5s %-20s %6s %17s %8s %5s\n", "-----", "--------------------", "---------", "-----------------", "-------",
           "---");
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (hashTable[i] != NULL) {
            printf("%5d %-20s %9s %18s %7d %5.1lf\n", i, hashTable[i]->name, hashTable[i]->number, hashTable[i]->email,
                   hashTable[i]->credits, hashTable[i]->gpa);
        }
    }
}

void displayRecord(struct student *temp, int index) {
//    printf("%s %s %s %d %lf", temp->name, temp->number, temp->email, temp->credits, temp->gpa);
}

void initializeTable(struct student *hashTable[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = 0;
    }
}

void insertStudent(struct student *hashTable[], struct student *temp) {
    // Do not forget to use the malloc() function to reserve space for the student structure
    // Do not forget to use the strcpy() function to store the string data in the hash table
    struct student *myTemp;
    bool isNull = true;

    int hashVal = computerHash(temp->name);
    int target = hashVal;
    int actual = hashVal;

    while (isNull) {
        if (hashTable[hashVal] == NULL) {
            myTemp = malloc(sizeof(struct student));
            hashTable[hashVal] = myTemp;
            strcpy(hashTable[hashVal]->name, temp->name);
            strcpy(hashTable[hashVal]->number, temp->number);
            strcpy(hashTable[hashVal]->email, temp->email);
            hashTable[hashVal]->credits = temp->credits;
            hashTable[hashVal]->gpa = temp->gpa;
            printf("\nAdded student to Hash Table: %-30s Target %d, Actual %d\n", temp->name, target, actual);
            isNull = false;
        } else {
            if (hashTable[hashVal]->name == temp->name) {
                displayError(DUPLICATE_NAME, temp->name);
                exit(0);
            }
            hashVal++;
            actual = hashVal;
        }
    }

}

bool testEmailFormat(struct student temp) {
    char email[17];
    email[17] = temp.email;

    if ((email[0] == "u") && (email[9] == "@") && (email[13] == ".")) {
        if (email[10] == "u" && email[14] == "e") {return true;}
        else{return false;}
    } else {
        displayError(EMAIL_FORMAT, temp.email);
        return false;
    }

}

bool testStudentData(struct student temp) {
    if ((strlen(temp.name) < 5) || (strlen(temp.name) > 20)) {
        printf("Error: ");
        displayError(NAME_LENGTH_MIN, temp.name);
        return false;
    } else if (strlen(temp.number) != 9) {
        printf("Error: ");
        displayError(NUMBER_LENGTH, temp.number);
        return false;
    } else if (temp.number[0] != 'u') {
        printf("Error: ");
        displayError(NUMBER_FORMAT, temp.number);
        return false;
    } else if (strlen(temp.email) != 17) {
        printf("Error: ");
        displayError(EMAIL_LENGTH, temp.email);
        return false;
    } else if (temp.email[9] != '@') {
        printf("Error: ");
        displayError(EMAIL_FORMAT, temp.email);
        return false;
    } else if ((temp.credits < 0) || (temp.credits > 150)) {
        printf("Error: ");
        displayError(CREDIT_VALUE_EXCEEDS_MAX, temp.credits);
        return false;
    } else if ((temp.gpa < 0.0) || (temp.gpa > 4.0)) {
        printf("Error: ");
        displayError(GPA_VALUE_EXCEEDS_MAX, NULL);
        return false;
    }
    return true;
}