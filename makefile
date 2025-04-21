CC = gcc
CFLAGS = `sdl2-config --cflags` -Wall
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC = main.c player.c bullet.c enemy.c enemy_bullet.c
OBJ = $(SRC:.c=.o)
OUT = main

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f *.o $(OUT)
