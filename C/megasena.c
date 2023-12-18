
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

jmp_buf buf;


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
        status = CLOSING;
    }

    if(VERBOSE)
        printf(message[status], urandom_path);

    if(status == CANT_REACH)
        longjmp(buf, CANT_REACH);  // back to main()

    return;
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

    FILE* fp = NULL;
    unsigned long long random_num;
    int count = 0;

    urandom_access(&fp);  // opening

    while(count < Q_NUM) {

        fread(&random_num, sizeof(random_num), 1ULL , fp);
        random_num = random_num % MAX_NUM + 1ULL;

        if(already_drawn(random_num, array) == true)
            continue;
        else
            array[count++] = random_num;
    }

    urandom_access(&fp);  // closing

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
        printf("%llu  ", array[i]);

    printf("\n");

    return;
}


int main() {

    unsigned long long array[Q_NUM] = {0ULL};

    for(int i = 0; i < GAMES_NUM; i++) {

        for(int i = 0; i < Q_NUM; i++)
            array[i] = 0ULL;  // reset the array

        if (setjmp(buf) == false) {  // try this code block
            mega_sena(array);
            sort_array(array);
            print_result(array);
        }

        else
            printf("\nSomething went wrong, exiting...\n");
    }

    // printf("\n");

    return 0;
}
