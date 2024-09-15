![image](https://github.com/user-attachments/assets/24853401-f382-4759-9692-94a429350938)
![image](https://github.com/user-attachments/assets/4bfe47b4-1e2f-4dbb-868c-4cde8253f8e4)

Tic tac toe game for Nintendo Gameboy made in C with GBDK (GameBoy Development Kit) library. Sprites drawn using GameBoy Tile Designer: https://www.devrs.com/gb/hmgd/gbtd.html

Tic-Tac-Toe.gb file is playable on original GameBoy hardware or on any GameBoy emulator.

AI player uses minimax algorythm that ensures the AI either wins or ties the game. Also uses alpha beta pruning to optimize minimax as base algorythm runtime was not acceptable on gameboy hardware. 

Compile with command 'make' in this directory. The Makefile will automatically detect and compile new source files as long 
as they are placed in the same directory as the Makefile


