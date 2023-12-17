
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define STR_TEXT_SIZE 50

// change the testing type easily, one spot
typedef unsigned short wildcard_type;  // try change short to: char, int, long int, long long int
#define MASK 0x01U  // 0x01U, 0x01L, 0x01LL, 0x01UL, 0x01ULL
char f_specifier[] = "%hu";  // %hhu, %hu, %u, %d, %ld, %lld, %lu, %llu


bool valid_range(long long int num, unsigned char last_bit, const char* vr_option) {

    bool result = false;

    if(strcmp("position", vr_option) == 0)
        result = num >= 0 && num <= last_bit;
    else
        result = num >= 0 && num < pow(2, last_bit + 1);  // true if it's inside valid range

    return result;
}


long long int read_input(const char* text_str, unsigned char last_bit, const char* vr_option) {

    long long int input_number;

    while(true) {

        printf("%s", text_str);
        scanf("%lld", &input_number);

        if(valid_range(input_number, last_bit, vr_option) == true)
            break;
        else
            printf("Invalid number for the chosen type (negative or too high), try again.\n");
    }

    return input_number;
}


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


bool read_bit(wildcard_type num, unsigned char n_bit) {

    return num & (MASK << n_bit);
}


void set_bit(wildcard_type* p_num, unsigned char n_bit) {

    *p_num = *p_num | (MASK << n_bit);

    return;
}


int main() {

    wildcard_type num;

    unsigned char n_bit, last_bit;  // you can use char, but left/right shift operator will promote to int either way... see documentation/drafts

    char last_bit_str[STR_TEXT_SIZE];

    bool is_set = false;


    // define last_bit

    last_bit = 8 * (unsigned char) sizeof(wildcard_type) - 1;
    sprintf(last_bit_str, "\nEnter the bit position to set (0...%d): ", last_bit);


    // number input

    num = (wildcard_type) read_input("Enter an integer number: ", last_bit, "number input");

    print_binary(num, last_bit);


    // bit position input 

    n_bit = (char) read_input(last_bit_str, last_bit, "position");


    // verifying if bit is set

    is_set = read_bit(num, n_bit);

    printf("\nThe bit %d of this number is %s.\n", n_bit, is_set ? "ALREADY SET, does not need to set.." : "NOT SET");


    // setting the bit

    if(is_set == false) {

        set_bit(&num, n_bit);

        char output_str_tmp[STR_TEXT_SIZE] = "\nNumber after setting the bit %d: ";
        strcat(output_str_tmp, f_specifier);  // add the format specifier of num variable to the string
        strcat(output_str_tmp, "\n");

        printf(output_str_tmp, n_bit, num);

        print_binary(num, last_bit);
    }

    return 0;
}
