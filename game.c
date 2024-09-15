#include <rand.h>
#include <stdio.h>
#include <stdint.h>
#include <gb/gb.h>
#include "gameBoardBackground.h"
#include "gameBoardBackgroundTileset.h"
#include "gameSprites2.h"
#include "windowmap.h"

int GAME_BOARD[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
int CURSOR_X = 0;
int CURSOR_Y = 0;
int cpuFirstTurn = 1;

void clearBoard(void){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            GAME_BOARD[i][j] = 0;
        }
    }
}

int validMove(int x, int y){
     return (GAME_BOARD[x][y] != 0) ? 0 : 1;
}

int checkGameOVER(int realCheck){
    

    for (int i = 0; i < 3; i++) {
        if (GAME_BOARD[i][0] != 0 && GAME_BOARD[i][0] == GAME_BOARD[i][1] && GAME_BOARD[i][1] == GAME_BOARD[i][2]) {
            if(realCheck){
                switch (i)
                {
                case 0:
                    set_bkg_tiles(0, 0, 20, 18, vertLeft);
                    break;
                case 1:
                    set_bkg_tiles(0, 0, 20, 18, vertMid);
                    break;
                case 2:
                    set_bkg_tiles(0, 0, 20, 18, vertRight);
                    break;
                }
            }
            return GAME_BOARD[i][0];
        }
        if (GAME_BOARD[0][i] != 0 && GAME_BOARD[0][i] == GAME_BOARD[1][i] && GAME_BOARD[1][i] == GAME_BOARD[2][i]) {
            if(realCheck){
                switch (i)
                {
                case 0:
                    set_bkg_tiles(0, 0, 20, 18, horTop);
                    break;
                case 1:
                    set_bkg_tiles(0, 0, 20, 18, horMid);
                    break;
                case 2:
                    set_bkg_tiles(0, 0, 20, 18, horBot);
                    break;
                }
            }
            return GAME_BOARD[0][i];
        }
    }

    // Check diagonals for a win
    if (GAME_BOARD[0][0] != 0 && GAME_BOARD[0][0] == GAME_BOARD[1][1] && GAME_BOARD[1][1] == GAME_BOARD[2][2]) {
        if(realCheck){
            set_bkg_tiles(0, 0, 20, 18, diagLeft);
        }
        return GAME_BOARD[0][0];
    }
    if (GAME_BOARD[0][2] != 0 && GAME_BOARD[0][2] == GAME_BOARD[1][1] && GAME_BOARD[1][1] == GAME_BOARD[2][0]) {
        if(realCheck){
            set_bkg_tiles(0, 0, 20, 18, diagRight);
        }
        return GAME_BOARD[0][2];
    }


    int isFull = 1;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(GAME_BOARD[i][j] == 0){
                isFull = 0;
                break;
            }
        }
    }
    if(isFull){
        return 3;
    }

    return 0;
}

int minimax(int depth, int isMaximizing, int alpha, int beta) {
    // Check if move ends game
    int result = checkGameOVER(0);
    int player;
    int bestScore;

    // If x wins
    if (result == 1) {
        return -1;
    }
    // If o wins
    if (result == 2) {
        return 1;
    }
    // If tie
    if (result == 3) {
        return 0;
    }

    // Game not over
    if (isMaximizing) {
        player = 2;
        bestScore = -1;
    } else {
        player = 1;
        bestScore = 1000;
    }

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (validMove(x, y)) {
                GAME_BOARD[x][y] = player;
                int score = minimax(depth + 1, 1 ^ isMaximizing, alpha, beta);
                GAME_BOARD[x][y] = 0;

                if (isMaximizing) {
                    if (score > bestScore) {
                        bestScore = score;
                    }
                    alpha = (alpha > bestScore) ? alpha : bestScore;
                } else {
                    if (score < bestScore) {
                        bestScore = score;
                    }
                    beta = (beta < bestScore) ? beta : bestScore;
                }

                // Alpha-beta pruning
                if (beta <= alpha) {
                    return bestScore;
                }
            }
        }
    }
    return bestScore;
}

void cpuTurnBest(void) {
    int bestScore = -1;
    int bestx = 1;
    int besty = 0;
    if (cpuFirstTurn && GAME_BOARD[1][1] == 0) {
        cpuFirstTurn = 0;
        GAME_BOARD[1][1] = 2;
        set_sprite_tile(1 + 3 * 1, PLAYERO);
        return;
    }
    if (cpuFirstTurn) {
        cpuFirstTurn = 0;
        int randNumber = rand() % 4;
        switch (randNumber)
        {
        case 0:
            GAME_BOARD[0][0] = 2;
            set_sprite_tile(0 + 3 * 0, PLAYERO);
            break;
        case 1:
            GAME_BOARD[0][2] = 2;
            set_sprite_tile(0 + 3 * 2, PLAYERO);
            break;
        case 2:
            GAME_BOARD[2][0] = 2;
            set_sprite_tile(2 + 3 * 0, PLAYERO);
            break;
        case 3:
            GAME_BOARD[2][2] = 2;
            set_sprite_tile(2 + 3 * 2, PLAYERO);
            break;
        }
        return;
    }
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            if (validMove(x, y)) {
                GAME_BOARD[x][y] = 2;
                int score = minimax(0, 0, -1, 1);
                GAME_BOARD[x][y] = 0;

                if (score > bestScore) {
                    bestScore = score;
                    bestx = x;
                    besty = y;
                }
            }
        }
    }
    GAME_BOARD[bestx][besty] = 2;
    set_sprite_tile(bestx + 3 * besty, PLAYERO);
}


void playerTurn(void){
    int key;

    while(1){
        //Multi tile select sprite
        set_sprite_tile(9, SELECT);
        move_sprite(9, ((((CURSOR_X)+1)*4)*8)+8, ((((CURSOR_Y)+1)*4-1)*8) + 16);

        set_sprite_tile(10, SELECT);
        move_sprite(10, (((((CURSOR_X)+1)*4)+2)*8)+8, ((((CURSOR_Y)+1)*4-1)*8) + 16);
        set_sprite_prop(10,S_FLIPX);
        

        set_sprite_tile(11, SELECT);
        move_sprite(11, ((((CURSOR_X)+1)*4)*8)+8, (((((CURSOR_Y)+1)*4-1)+2)*8) + 16);
        set_sprite_prop(11,S_FLIPY);

        set_sprite_tile(12, SELECT);
        move_sprite(12, (((((CURSOR_X)+1)*4)+2)*8)+8, (((((CURSOR_Y)+1)*4-1)+2)*8) + 16);
        set_sprite_prop(12,S_FLIPX|S_FLIPY);

        waitpadup();
        key = waitpad(J_LEFT | J_RIGHT | J_UP | J_DOWN | J_A);

        if(key & J_LEFT && CURSOR_X > 0){
            CURSOR_X -= 1;
        }
        if(key & J_RIGHT && CURSOR_X < 2){
            CURSOR_X += 1;
        }
        if(key & J_UP && CURSOR_Y > 0){
            CURSOR_Y -= 1;
        }
        if(key & J_DOWN && CURSOR_Y < 2){
            CURSOR_Y += 1;
        }
        if(key & J_A && validMove(CURSOR_X, CURSOR_Y)){


            NR21_REG = 0x81; 
            NR22_REG = 0x84;
            NR23_REG = 0xD7;  
            NR24_REG = 0x86;  
            GAME_BOARD[CURSOR_X][CURSOR_Y] = 1;
            set_sprite_tile(CURSOR_X+3*CURSOR_Y, PLAYERX);
            break;
        }
    }
}


void gameplay(void){
    set_win_tiles(0,0,11,1, noText);
    clearBoard();
    CURSOR_X = 0;
    CURSOR_Y = 0;
    cpuFirstTurn = 1;

    for(int i = 0; i < 9; i++){
        set_sprite_tile(i, BLANK);
    }

    while(1){
        playerTurn();
        if(checkGameOVER(1)){
             break;
        }
        cpuTurnBest();
        if(checkGameOVER(1)){
             break;
        }

    }

    if(checkGameOVER(1) == 1){
        set_win_tiles(0,0,11,1, playerWins);
    }

     if(checkGameOVER(1) == 2){
        set_win_tiles(0,0,11,1, cpuWins);
    }
    if(checkGameOVER(1) == 3){
        set_win_tiles(0,0,11,1, tiedGame);
    }
    
    
    set_sprite_tile(9, BLANK);
    set_sprite_tile(10, BLANK);
    set_sprite_tile(11, BLANK);
    set_sprite_tile(12, BLANK);
    waitpadup();
    waitpad(J_A);
    waitpadup();
}