
#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARACTERS_PER_LINE 50
#define NUMBERS_PER_LINE 5
#define NUMBERS_PER_FILE 5 * 4

// some random numbers, they will go to a file
const char* const numbers_str = "85    11   213    90    7\n"
                                "41     2   -19    58   12\n"
                                "-20  301    28   591   11\n"
                                "99    35   -10    40    1";


// wrapper for fopen()
FILE* open_file(const char* filename, const char* mode) {

    FILE* fp = NULL;

    if ((fp = fopen(filename, mode)) == NULL) {

        fprintf(stdout, "Oops, something wrong opening %s file, exiting...", filename);
        exit(EXIT_FAILURE);
    }
    
    return fp;
}


int main() {

    char line[MAX_CHARACTERS_PER_LINE] = {'\0'};  // string
    int numbers[NUMBERS_PER_LINE] = { 0 };
    int num = 0;

    char* filename = "numbers.txt";
    FILE* fp = NULL;

    // writing the file

    fp = open_file(filename, "w");

    fputs(numbers_str, fp);
    fclose(fp);

    // reading the file

    fp = open_file(filename, "r");

    for (int i = 0; i < NUMBERS_PER_FILE ; i++) {

        fscanf(fp, "%d", &num);  // fscanf() will advances the pointer in the text file

        if (num % 2 == 0)
            fprintf(stdout, "number %d is even\n", num);
        else
            fprintf(stdout, "number %d is odd\n", num);
    }

    /*

    // Using fgets() + sscanf()

    while (fgets(line, (int)sizeof(line), fp) != NULL) {  // remember, fgets() adds \0 in the end of the line/string

        // puts(line);

        sscanf(line, "%d\t%d\t%d\t%d\t%d\t", &numbers[0], &numbers[1], &numbers[2], &numbers[3], &numbers[4]);

        for (int i = 0; i < NUMBERS_PER_LINE; i++) {

            if (numbers[i] % 2 == 0)
                fprintf(stdout, "number %d is even\n", numbers[i]);
            else
                fprintf(stdout, "number %d is odd\n", numbers[i]);
        }
    }

    */

    fclose(fp);

    return 0;
}
