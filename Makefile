CC = gcc
CFLAGS= -O3 -funroll-loops -march=native
fuckbrainfuck: main.c utils.c core.c args.c
	$(CC) -o bf main.c utils.c core.c args.c $(CFLAGS)

