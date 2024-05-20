CC=gcc
emuchip8:
	$(CC) -o chip8 -g cpu.c font.c main.c -lSDL2
clean:
	rm ./chip8
