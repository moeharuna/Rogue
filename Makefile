CC = gcc
FLAGS = -DDEBUG -Og -Wall -Wpedantic
FILES = src/level.c src/rogue.c
OUT   = out

all:
	$(CC) $(FLAGS) $(FILES) -o $(OUT)
