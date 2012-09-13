all: main

main: main.c
	gcc -std=c99 -O2 -o oc_convert main.c
