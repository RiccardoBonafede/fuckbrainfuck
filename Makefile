CC = gcc

fuckbrainfuck: main.c utils.c core.c
	$(CC) -o bf main.c utils.c core.c