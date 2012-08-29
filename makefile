SRC = $(wildcard *.c)
CFLAGS = -Wall -ansi -g -std=c99 -lm
OUTPUT = binary
CC = gcc
all:
	@$(CC) $(CFLAGS) $(SRC) -o binary
clean:
	@rm -rf *.o *.s *.zip
zip: 
	@zip fonte.zip *.c *.h

megazip:
	@zip all.zip *