
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#define MASK 0x01

// change the testing type easily, one spot
typedef unsigned short wildcard_type;
#define PRINT(x) printf("\nNumber after setting the bit %d: %hu\n", n_bit, *p_num);  // %hhu, %hu, %u, %d, %ld, %lld


void print_binary(wildcard_type num, unsigned char last_bit) {

    printf("binary representation: ");

    for(int i = last_bit; i >= 0; i--) {

        printf("%d", num & (MASK << i) ? 1: 0);

        if(i % 4 == 0)
            printf(" ");
    }

    printf("\n");

    return;
}

bool read_bit(wildcard_type num, char n_bit) {

    return num & (MASK << n_bit);
}

void set_bit(wildcard_type* p_num, char n_bit) {

    *p_num = *p_num | (MASK << n_bit);
    PRINT(*p_num);

    return;
}

int main() {

    wildcard_type num;

    unsigned char n_bit, last_bit;  // you can use char, but left/right shift operator will promote to int either way... see documentation/drafts
    last_bit = 8 * (unsigned char)sizeof(wildcard_type) - 1;

    bool is_set = false;

    char* set_text_array[] = {"ALREADY SET, does not need to set..", "NOT SET"};

    // input number

    while(true) {
        long long int num_tmp;
        printf("Enter an integer number: ");
        scanf("%lld", &num_tmp);

        if(num_tmp < (long long int)pow(2, last_bit + 1)) {
            num = (wildcard_type)num_tmp;
            break;
        }
        else
            printf("Invalid number for the chosen type (too high), try again.\n");
    }

    print_binary(num, last_bit);

    // input bit position

    do {
        printf("\nEnter the bit position to set (0...%d): ", last_bit);
        scanf("%hhu", &n_bit);
    } while(n_bit < 0 || n_bit > last_bit);

    // verifying if bit is set

    is_set = read_bit(num, n_bit);

    printf("\nThe bit %d of this number is %s.\n", n_bit, is_set ? set_text_array[0] : set_text_array[1]);

    // setting the bit

    if(is_set == false) {
        set_bit(&num, n_bit);
        print_binary(num, last_bit);
    }

    return 0;
}
