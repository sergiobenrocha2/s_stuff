
/*

max characters per line is 256

program accepts 2 parameters, first is a character, second is the file it will search the lines that contains the characater
./myprogram x my_file.txt
 
*/

#define _GNU_SOURCE  // gcc only

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_PER_LINE (size_t)256


int main(int argc, char* argv[]) {

    size_t elements_of_buf = MAX_CHAR_PER_LINE;  // unsigned
    char* buf = NULL;
    FILE* fp = NULL;

    if (argc != 3) {
        puts("\nOops! Wrong usage of this program, it needs 2 arguments.\n");
        exit(EXIT_FAILURE);
    }

    buf = (char* ) calloc(elements_of_buf, sizeof(char));

    fp = fopen(argv[2], "r");

    // while (getline(&buf, &elements_of_buf, fp) != EOF) {  // gcc compatible only
    while (fgets(buf, (int)elements_of_buf, fp) != NULL) {  // more portable

        if (strchr(buf, argv[1][0]))
            fputs(buf, stdout); // puts(buf);
        
        for (int i = 0; i < elements_of_buf; i++)
            buf[i] = '\0';  // clear the buf
    }

    free(buf);
    buf = NULL;

    fclose(fp);

    return 0;
}
