
/* we should use some Regex in the input... but works for now */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_NUM 20

enum type {CHAR, INT, FLOAT};

struct student {
    enum type grade_type;
    union {
        char letterGrade;
        int roundedGrade;
        float exactGrade;
    };
};

int main() {

    struct student variable_1, variable_2;

    char input_buf[MAX_NUM] = "\0";


    // variable_1 test

    fgets(input_buf, sizeof(input_buf), stdin);

    if (strchr(input_buf, (int)'.') != NULL)
        sscanf(input_buf, "%f", &variable_1.exactGrade);
    
    else if (isalpha(input_buf[0]) != 0)
        sscanf(input_buf, "%c", &variable_1.letterGrade);
    
    else
        sscanf(input_buf, "%d", &variable_1.roundedGrade);

    printf("\n");
    printf("%c\n", variable_1.letterGrade);
    printf("%d\n", variable_1.roundedGrade);
    printf("%f\n", variable_1.exactGrade);


    // variable_2 test

    variable_2.letterGrade = 'A';
    printf("\n%c", variable_2.letterGrade);

    variable_2.roundedGrade = 10;
    printf("\n%d", variable_2.roundedGrade);

    variable_2.exactGrade = 10.0f;
    printf("\n%.1f", variable_2.exactGrade);

    return 0;
}

