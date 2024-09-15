#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "gameBoardBackground.h"
#include "gameBoardBackgroundTileset.h"
#include "gameSprites2.h"

void title_screen(void){
    set_sprite_tile(0, LETTERT);
    set_sprite_tile(1, LETTERI);
    set_sprite_tile(2, LETTERC);
    set_sprite_tile(3, LETTERT);
    set_sprite_tile(4, LETTERA);
    set_sprite_tile(5, LETTERC);
    set_sprite_tile(6, LETTERT);
    set_sprite_tile(7, LETTERO);
    set_sprite_tile(8, LETTERE);
    set_sprite_tile(9, BLANK);
    waitpad(J_A | J_B | J_START | J_SELECT);
    waitpadup();

}