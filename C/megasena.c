
/* Works only on Linux, for multiplatform we should use libsodium lib 
 * For plotting: GNUplot, PGPLOT, PLplot, Cplotlib, SDL, OpenGL
 */

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#define VERBOSE false
#define MAX_NUM 60ULL
#define Q_NUM 6
#define GAMES_NUM 3

enum random_gen_cmd_flags {START, CLOSE, _NONE, GENERATE};

static jmp_buf buf;  // only accessible by this file


/*
 * Function:  urandom_access
 * -------------------------
 *   Opens or closes the file /dev/urandom
 * 
 *   fp: a pointer to a file pointer, if provided NULL the function will open
 *       /dev/urandom, if non-NULL it will close /dev/urandom
 */

void urandom_access(FILE** fp) {

    enum status_flags {OPENING, CLOSING, CANT_REACH};  // 0: opening, 1: closing, 2: can't reach file

    enum status_flags status;  
    char urandom_path[] = "/dev/urandom";
    char* message[] = {"\nOpening %s", "\nClosing %s", "\nOuch! Can't access %s, exiting!"};

    if(*fp == NULL) {

        *fp = fopen(urandom_path, "r");
        status = OPENING;
        // *fp = NULL;  // dummy error, simulating fopen() return

        if(*fp == NULL)
            status = CANT_REACH;
    }

    else {

        fclose(*fp);
        *fp = NULL;
        status = CLOSING;
    }

    if(VERBOSE)
        printf(message[status], urandom_path);

    if(status == CANT_REACH)
        longjmp(buf, CANT_REACH);  // back to main()

    return;
}


/*
 * Function:  random_gen
 * ---------------------
 *   returns random number from /dev/urandom, needs to be initialized (START
 *   command)
 *
 *   cmd: command to the function, START, CLOSE, GENERATE
 */

unsigned long long random_gen(int cmd) {

    unsigned long long random_num = 0ULL;
    static FILE* fp = NULL;

    if(fp == NULL && cmd == START)
        urandom_access(&fp);  // opening

    else if(fp != NULL && cmd == GENERATE)
        fread(&random_num, sizeof(random_num), 1ULL , fp);

    else if(fp != NULL && cmd == CLOSE)
        urandom_access(&fp);  // closing

    else
        printf("Something wrong in random_gen()!");

    return random_num;
}


bool already_drawn(unsigned long long num, unsigned long long* array) {

    bool result = false;

    for(int i = 0; array[i] != 0; i++)

        if(num == array[i]) {
            result = true;
            break;
        }

    return result;
}


void mega_sena(unsigned long long array[]) {

    unsigned long long random_num;
    int count = 0;

    while(count < Q_NUM) {

        random_num = random_gen(GENERATE) % MAX_NUM + 1ULL;

        if(already_drawn(random_num, array) == true)
            continue;
        else
            array[count++] = random_num;
    }

    return;
}


void sort_array(unsigned long long array[]) {

    for(int i = 0; i < Q_NUM - 1; i++)

        for(int j = i + 1; j < Q_NUM; j++)

            if(array[i] > array[j]) {

                // swaping the elements using XOR operator:
                array[i] ^= array[j];
                array[j] ^= array[i];
                array[i] ^= array[j];
            }

    return;
}


void print_result(unsigned long long array[]) {

    printf("\nSeu jogo:  ");

    for(int i = 0; i < Q_NUM; i++)
        printf("%2llu  ", array[i]);

    printf("\n");

    return;
}


int main() {

    unsigned long long array[Q_NUM] = {0ULL};

    if (setjmp(buf) == false) {  // try this code block

        random_gen(START);  // initializing random generator

        for(int i = 0; i < GAMES_NUM; i++) {

            for(int i = 0; i < Q_NUM; i++)
                array[i] = 0ULL;  // reset the array

            mega_sena(array);
            sort_array(array);
            print_result(array);
        }

        random_gen(CLOSE);  // closing the /dev/urandom
    }

    else {

        printf("\nSomething went wrong, exiting... switch VERBOSE to true for more information\n");
    }

    return 0;
}
