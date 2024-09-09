#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "gameBoardBackground.h"
#include "gameBoardBackgroundTileset.h"
#include "windowmap.h"
#include "gameSprites.h"
#include "title.h"
#include "game.h"
#include <gbdk/font.h>


void main(void){
    //Enables sound
    //Must be in this order
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels



    font_t min_font;
    font_init();
    min_font = font_load(font_min); //36 tiles
    font_set(min_font);

    //Set Background
    set_bkg_data(37, 8, backgroundTiles);
    set_bkg_tiles(0, 0, 20, 18, background);
    SHOW_BKG;
    
    set_win_tiles(0,0,11,1, pressStart);
    move_win(15, 130);
    SHOW_WIN;

    //Set Sprites
    set_sprite_data(0, 10, gameSprites);

    //Create multi-tile selector sprite
    set_sprite_tile(9, BLANK);
    move_sprite(9, ((((0%3)+1)*4)*8)+8, ((((0/3)+1)*4-1)*8) + 16);

    set_sprite_tile(10, BLANK);
    move_sprite(10, ((((0%3)+1)*4)*8)+8, ((((0/3)+1)*4-1)*8) + 16);

    set_sprite_tile(11, BLANK);
    move_sprite(11, ((((0%3)+1)*4)*8)+8, ((((0/3)+1)*4-1)*8) + 16);

    set_sprite_tile(12, BLANK);
    move_sprite(12, ((((0%3)+1)*4)*8)+8, ((((0/3)+1)*4-1)*8) + 16);

    //Create grid of 9 sprites for game
    for(int i = 0; i < 9; i++){
        set_sprite_tile(i, 0);
        move_sprite(i, ((1+((i%3)+1)*4)*8)+8, ((((i/3)+1)*4)*8) + 16);   
    }
    SHOW_SPRITES;

    //set_sprite_tile(0, LETTERT);
    

    
    
    // Loop forever
    while(1) {

	  
		// Game main loop processing goes here
        set_win_tiles(0,0,11,1, pressStart);
        title_screen();
        gameplay();
		// Done processing, yield CPU and wait for start of next frame
        vsync();
    }
}
