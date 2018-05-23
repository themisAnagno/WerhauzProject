# Makefile for the werhauz application
# *************************************

# Set Variables for the Makefile
CC = gcc
Flags = -g -Wall

# Targets
all: werhauz.exe

werhauz.exe: main.o menu.o strings.o file_menu_alt.o hash.o topk.o config.o
	$(CC) $(Flags) -o $@ $^

main.o: main.c
	$(CC) $(Flags) -c $^

menu.o: menu.c
	$(CC) $(Flags) -c $^

strings.o: strings.c
	$(CC) $(Flags) -c $^

file_menu_alt.o: file_menu_alt.c
	$(CC) $(Flags) -c $^	

hash.o: hash.c
	$(CC) $(Flags) -c $^

topk.o: topk.c
	$(CC) $(Flags) -c $^

config.o: config.c
	$(CC) $(Flags) -c $^