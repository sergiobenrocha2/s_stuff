
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DRAW 0
#define PLAYER_1 1
#define PLAYER_2 2
#define NO_WINNER_YET 12
#define PLAYER_1_MARK 'X'
#define PLAYER_2_MARK 'O'
#define ASCII_OFFSET 48


int checkForWin();
void drawBoard();
void markBoard(int move, int playerS);
int askPlayerMove();

char board[10] = "0123456789";

int main() {
    int move, winner, player = PLAYER_1;
    
    while(true) {
        drawBoard();
        
        move = askPlayerMove(player);
        markBoard(move, player);
        
        winner = checkForWin();
        if (winner != NO_WINNER_YET)
            break;
        
        player = (player == PLAYER_1) ? PLAYER_2 : PLAYER_1;
    }
    
    drawBoard();
    
    if (winner == DRAW)
        printf("\nIt's a draw!!!\n\n");
    else 
        printf("\nPlayer %d wins!\n\n", winner);
    
    return 0;
}

void drawBoard_spaces(void) {
    printf("\n   |   |   ");
}

void drawBoard_chars(char a, char b, char c) {
    printf("\n %c | %c | %c ", a, b, c);
}

void drawBoard_underlines(void) {
    printf("\n___|___|___");
}

void drawBoard(void) {
    system("@cls||clear");
    
    printf("\nTic Tac Toe");
    printf("\n\nPlayer 1 (%c) - Player 2 (%c)\n", PLAYER_1_MARK, PLAYER_2_MARK);
    drawBoard_spaces();
    drawBoard_chars(board[1], board[2], board[3]);
    drawBoard_underlines();
    drawBoard_spaces();
    drawBoard_chars(board[4], board[5], board[6]);
    drawBoard_underlines();
    drawBoard_spaces();
    drawBoard_chars(board[7], board[8], board[9]);
    drawBoard_spaces();
    printf("\n");
}

bool checkForWin_bool(int seq[3], char mark) {
    // printf("\n\n%d %d %d\n\n", seq[0], seq[1], seq[2]);
    return ((board[seq[0]] == mark) && (board[seq[1]] == mark) && (board[seq[2]] == mark));
}

// return values: see preprocessor above
int checkForWin(void) {
    char marks[3] = { ' ', PLAYER_1_MARK, PLAYER_2_MARK };
    int sequence[8][3] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {1, 4, 7}, {2, 5, 8}, {3, 6, 9}, {1, 5, 9}, {7, 5, 3} };  // sequencias validas
    char winner;
    int count = 0;
    
    for (int i = PLAYER_1; i <= PLAYER_2; i++) {
        winner = i;
        for (int j = 0; j < 8; j++) {
            if (checkForWin_bool(sequence[j], marks[i]) == true)
                goto checkForWin_return;
        }
        winner = NO_WINNER_YET;
    }
    
    for (int i = 0; i < 9; i++)
        if (board[i] == PLAYER_2_MARK)
            count++;
    if (count == 4)
        winner = DRAW;
    
    checkForWin_return: return winner;
}

void markBoard(int move, int player) {
    char mark;
    
    if (player == PLAYER_1)
        mark = PLAYER_1_MARK;
    else
        mark = PLAYER_2_MARK;
    
    board[move] = mark;
    
    return;
}

int askPlayerMove(int player) {
    int position;
    
    while (true) {
        printf("\nPlayer %d, enter a number: ", player);
        scanf("%d", &position);
        
        if (position < 1 || position > 9 || board[position] == PLAYER_1_MARK || board[position] == PLAYER_2_MARK) {
            printf("\nInvalid move! Try again\n");
            while (getchar() != '\n');  // flush the stdin
        }
        else
            break;
    }
    return position;
}


// outra forma de implementar a leitura, porém não está 100%:

/*
int askPlayerMove(int player) {
    int position;
    
    while (true) {
        printf("\nPlayer %d, enter a number: ", player);
        // scanf("%d", &position);
        while (true) {
            position = getchar() - ASCII_OFFSET;
            if ((position != '\n' || position != EOF) && position > 0)
                break;
        }
        if (position < 1 || position > 9);
        else if (board[position] == PLAYER_1_MARK || board[position] == PLAYER_2_MARK);
        else
            break;
        printf("\nInvalid move! Try again\n");
    }
    return position;
}
*/