
#include <stdio.h>
#include <stdbool.h>

#define STR_WORD_SIZE 16


enum fill_color {BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE};
enum b_style {SOLID, DOTTED, DASHED};

const char box_opaque_str[][STR_WORD_SIZE] = {"transparent", "opaque"};  // "transparent\0\0\0\0\0opaque\0\0\0\0\0\0\0\0\0\0"
// const char* const box_opaque_str[] = {"transparent", "opaque"};  // "transparent\0opaque\0"

const char* const fill_color_str[] = {"black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};
const char* const border_shown_str[] = {"hidden", "shown"};
const char* const border_style_str[] = {"solid", "dotted", "dashed"};


struct box_properties {

    // first Byte
    unsigned short box_opaque: 1;  // opaque --> true or transparent --> false
    unsigned short box_color: 3;

    unsigned short :4;  // padding

    // second Byte
    unsigned short border_shown: 1;  // shown --> true or hidden --> false
    unsigned short border_color: 3;
    unsigned short border_style: 2;  // solid, dotted, dashed
};


int main() {

    struct box_properties test = {true, BLUE, false, .border_style=DASHED};
    // struct box_properties test = {true, BLUE, false, BLACK, DASHED};

    /*
    struct box_properties test;

    test.box_opaque = true;
    test.box_color = BLUE;
    test.border_shown = false;
    test.border_style = DASHED;
    */

    enum fill_color my_color = WHITE;

    test.border_color = my_color;

    printf("%d  %d  %d  %d  %d\n", test.box_opaque,
                                   test.box_color,
                                   test.border_shown,
                                   test.border_color,
                                   test.border_style);

    printf("%s  %s  %s  %s  %s\n", box_opaque_str[test.box_opaque],
                                   fill_color_str[test.box_color],
                                   border_shown_str[test.border_shown],
                                   fill_color_str[test.border_color],
                                   border_style_str[test.border_style]);

    printf("\nSize of the struct: %zu bits\n", 8 * sizeof(struct box_properties));

    return 0;
}
