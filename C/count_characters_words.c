
#include <stdio.h>
#include <stdbool.h>

struct count {
    int characters;
    int words;
};


/*
 * Function:  create_file
 * -------------------------
 *   Creates a test file
 * 
 */

void create_file(void) {

    FILE* pFile = NULL;
    char some_string[] = "Test text here\n\nlast words";

    pFile = fopen("myFile.txt", "w");

    if (pFile != NULL) {

        for (int i = 0 ; some_string[i] != '\0'; i++)
            fputc(some_string[i], pFile);
    }

    fclose(pFile);

    return;
}


/*
 * Function:  read_input
 * -------------------------
 *   Reads input from stdin (default) or from a file
 * 
 *   fp: a file pointer
 * 
 */

struct count read_input(FILE* fp) {

    struct count my_count = {0, 0};
    int ch = 0;
    int last_ch = (int)' ';  // in case file is empty filled with spaces or \n

    bool is_delimiter = false;
    bool is_last_ch_alphanum = false;
    bool is_first_delimiter = false;

    if (fp == NULL)
        fp = stdin;

    while (ch != EOF) {  // EOF is -1

        ch = fgetc(fp);

        is_delimiter = ch == ' ' ||   // C implicitly converts chars to ASCII int value
                       ch == '\n' ||
                       ch == '\r' ||  // carriage return for windows files
                       ch == EOF;

        is_last_ch_alphanum = last_ch != ' ' &&
                              last_ch != '\n' &&
                              last_ch != '\r' &&
                              last_ch != EOF;  // last_ch == a-z, A-Z, 0-9 or symbols + - ! @...

        is_first_delimiter = (is_last_ch_alphanum == true) &&
                             (is_delimiter == true);


        if (is_delimiter == false)
            my_count.characters++;

        if (is_delimiter == true && is_first_delimiter == true)
            my_count.words++;

        last_ch = ch;

        // printf("%d ", ch);  // print ASCII values
        // if (ch == '\n')
        //     printf("\n");
    }

    return my_count;
}


/*
 * Function:  read_from_file
 * -------------------------
 *   Wrapper for read_input() function
 * 
 *   file_name: string / char array that contains the file name to be opened
 * 
 */
 
struct count read_from_file(char* file_name) {

    struct count my_count = {0, 0};
    FILE* pf = NULL;

    if ((pf = fopen(file_name, "r")) != NULL) {
        my_count = read_input(pf);
        fclose(pf);
    }

    else {
        printf("\nOops something wrong, can't open the file\n");
    }   

    return my_count;
}


int main(int argc, char* argv[]) {

    // create_file();

    struct count count_num = {0, 0};

    if (argc == 1) {
        printf("\nTo exit, press Ctrl+D twice (it will send EOF to the program):\n\n");
        count_num = read_input(NULL);
        printf("\n");
    }

    else if (argc == 2)
        count_num = read_from_file(argv[1]);

    else
        printf("\nToo many arguments passed!! Try again, use only 1 argument (1 file).\n");

    printf("\nNumber of characters: %d\nNumber of words: %d\n\n", count_num.characters, count_num.words);

    return 0;
}


/*
 * To call the program in terminal with a file as parameter:
 * 
 * ./count_characters_words myFile.txt
*/