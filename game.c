#include <rand.h>
#include <stdio.h>
#include <stdint.h>
#include <gb/gb.h>
#include "gameBoardBackground.h"
#include "gameBoardBackgroundTileset.h"
#include "gameSprites.h"
#include "windowmap.h"

int GAME_BOARD[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
int CURSOR_X = 0;
int CURSOR_Y = 0;

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

int checkGameOVER(void){
    

    for (int i = 0; i < 3; i++) {
        if (GAME_BOARD[i][0] != 0 && GAME_BOARD[i][0] == GAME_BOARD[i][1] && GAME_BOARD[i][1] == GAME_BOARD[i][2]) {
            return GAME_BOARD[i][0];
        }
        if (GAME_BOARD[0][i] != 0 && GAME_BOARD[0][i] == GAME_BOARD[1][i] && GAME_BOARD[1][i] == GAME_BOARD[2][i]) {
            return GAME_BOARD[0][i];
        }
    }

    // Check diagonals for a win
    if (GAME_BOARD[0][0] != 0 && GAME_BOARD[0][0] == GAME_BOARD[1][1] && GAME_BOARD[1][1] == GAME_BOARD[2][2]) {
        return GAME_BOARD[0][0];
    }
    if (GAME_BOARD[0][2] != 0 && GAME_BOARD[0][2] == GAME_BOARD[1][1] && GAME_BOARD[1][1] == GAME_BOARD[2][0]) {
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

void cpuTurn(void){
    while(1){
        int randX = rand() % 3;
        int randY = rand() % 3;
        if(validMove(randX, randY)){
            GAME_BOARD[randX][randY] = 2;
            set_sprite_tile(randX+3*randY, PLAYERO);
            break;
        }
    }
    
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

    for(int i = 0; i < 9; i++){
        set_sprite_tile(i, BLANK);
    }

    while(1){
        playerTurn();
        if(checkGameOVER()){
             break;
        }
        cpuTurn();
        if(checkGameOVER()){
             break;
        }

    }

    if(checkGameOVER() == 1){
        set_win_tiles(0,0,11,1, playerWins);
    }

     if(checkGameOVER() == 2){
        set_win_tiles(0,0,11,1, cpuWins);
    }
    if(checkGameOVER() == 3){
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