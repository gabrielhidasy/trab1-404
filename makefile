SRC = $(wildcard *.c)
CFLAGS = -Wall -ansi -g -std=c99 -lm -O3
OUTPUT = binary
CC = gcc
SUSYCFLAGS = -std=c99 -pedantic -Wall -lm 
all:
	@$(CC) $(CFLAGS) $(SRC) -o montador-ias
susy:
	@$(CC) $(SUSYCFLAGS) $(SRC) -o montador-ias
clean:
	@rm -rf *.o *.s *.zip
zip: 
	@zip fonte.zip *.c *.h

megazip:
	@zip -r all.zip * 
tudo:
	./tester.sh
	make all
	make zip
	make megazip
