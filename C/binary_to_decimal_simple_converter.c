
#include <stdio.h>
#include <math.h>
#include <limits.h>

#define MAX_NUM_BITS 10

int decimal_to_binary(int num_dec) {

    int num_bin = 0, i = 0;

    while(num_dec != 0) {
        
        num_bin += (num_dec % 2) * (int)pow(10, i);
        num_dec = (int)(num_dec / 2);
        i++;
    }

    return num_bin;
}

int binary_to_decimal(int num_bin) {

    int num_dec = 0;

    for(int i = 0; i < MAX_NUM_BITS; i++) {
        
        if(num_bin % 2 == 1) {
            num_bin = num_bin - 1;
            num_dec += pow(2, i);
        }
        
        num_bin = num_bin / 10;
        
        if(num_bin == 0)
            break;
    }

    return num_dec;
}

int main() {
    
    // int x = 1111111111;  // max binary using int
    // printf("\n%d\n\n", INT_MAX);  // 2147483647
    
    int bin = 101;
    int dec = 10;
    
    printf("\n0b%d = %d", bin, binary_to_decimal(bin));
    printf("\n%d = 0b%d", dec, decimal_to_binary(dec));

    return 0;
}

