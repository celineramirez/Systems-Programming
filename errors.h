#pragma once // Prevents the header file from loading multiple times

enum Error {
    MISSING_COMMAND_LINE_ARG,
    FILE_NOT_FOUND,
    BLANK_RECORD,
    CORRUPT_RECORD,
    SHORT_RECORD,
    DUPLICATE_NAME,
    NAME_LENGTH_MIN,
    NUMBER_LENGTH,
    NUMBER_FORMAT,
    EMAIL_LENGTH,
    EMAIL_FORMAT,
    CREDIT_VALUE_EXCEEDS_MAX,
    GPA_VALUE_EXCEEDS_MAX,
};

void displayError(enum Error errors, char* errorInfo) {
    switch(errors){

        case MISSING_COMMAND:
            printf("Missing command ");
            break;

        case FILE_NOT_FOUND:
            printf("File was not found ");
            break;

        case BLANK_RECORD:
            printf("Blank record ");
            break;

        case CORRUPT_RECORD:
            printf("Corrupt record ");
            break;

        case SHORT_RECORD:
            printf("Short record ");
            break;

        case DUPLICATE_NAME:
            printf("Duplicate name ");
            break;

        case NAME_LENGTH_MIN:
            printf("Student name not between 5 and 20 characters ");
            break;

        case NUMBER_LENGTH:
            printf("Number length ");
            break;
        case NUMBER_FORMAT:
            printf("Number length ");
            break;
        case EMAIL_LENGTH:
            printf("Email length ");
            break;

        case EMAIL_FORMAT:
            printf("Student email format is invalid ");
            break;

        case CREDIT_VALUE_EXCEEDS_MAX:
            printf("Student credit not less than 150 ");
            break;

        case GPA_VALUE_EXCEEDS_MAX:
            printf("GPA value exceeds maximum ");
            break;

        default:
            break;
    }


}