
/* Works only on Linux, for multiplatform we should use libsodium lib 
 * For plotting: GNUplot, PGPLOT, PLplot, Cplotlib, SDL, OpenGL
 */

#define USE_GETRANDOM false
#define VERBOSE false

#include <stdio.h>
#include <stdbool.h>
#include <setjmp.h>

#if USE_GETRANDOM
#include <sys/random.h>
#endif


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


void print_binary(unsigned long long num) {

    #define MASK 0x01ULL
    int last_bit = 8 * (int) sizeof(num) - 1;

    printf("\n--> ");

    for(int i = last_bit; i >= 0; i--) {

        printf("%d", num & (MASK << i) ? 1: 0);

        if(i % 4 == 0)
            printf(" ");
        if(i % 8 == 0)
            printf(" ");
    }

    return;
}


unsigned long long random_gen(int cmd) {

    unsigned long long random_num = 0ULL;
    static FILE* fp = NULL;

    if(fp == NULL && cmd == START)
        urandom_access(&fp);  // opening

    else if(fp != NULL && cmd == GENERATE) {

        #if USE_GETRANDOM
            ssize_t result_from_getrandom;
            if((result_from_getrandom = getrandom(&random_num, sizeof(random_num), GRND_NONBLOCK)) != sizeof(random_num)) {
                printf("\n--> %zd Bytes were read!\n", result_from_getrandom);
                longjmp(buf, -1);  // error, back to main()
            }
        #else
            size_t count = (size_t) 1;
            if(fread(&random_num, sizeof(random_num), count, fp) < count) {  // if less than count param of fread()
                printf("\n--> 0 Byte was read!\n");
                longjmp(buf, -1);
            }
        #endif

        if(VERBOSE) {
            printf("\nrandom number:\n--> %llu", random_num);
            print_binary(random_num);
        }
    }

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

/*
 * Function:  create_csv_from_N_runs
 * ---------------------------------
 *   creates a CSV file filled with random numbers (TEST_NUM rows, Q_NUM cols),
 *   so you can load easily this data with Python / NumPy. Numbers are not
 *   sorted.
 *
 */

void create_csv_from_N_runs(void) {

    #define TEST_NUM 100000

    unsigned long long array_2[TEST_NUM][Q_NUM] = {0ULL};
    FILE* fp_w = NULL;

    fp_w = fopen("test.csv", "w");

    random_gen(START);

    for(int i = 0; i < TEST_NUM; i++)
        mega_sena(array_2[i]);

    for(int i = 0; i < TEST_NUM; i++) {
        for(int j = 0; j < Q_NUM; j++) {
            fprintf(fp_w, "%llu", array_2[i][j]);
            if(j != Q_NUM - 1)
                fprintf(fp_w, ",");
        }
        fprintf(fp_w, "\n");
    }

    fclose(fp_w);
    fp_w = NULL;

    random_gen(CLOSE);

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

    // create_csv_from_N_runs();

    return 0;
}
