SRC = $(wildcard *.c)
PROFILINGCFLAGS = -Wall -ansi -g -lm -Os -pedantic -std=c99 -pg
CFLAGS = -Wall -ansi -g -lm -O3 -pedantic -std=c99
OUTPUT = binary
CC = gcc
SUSYCFLAGS = -std=c99 -pedantic -Wall -lm 
all:
	@$(CC) $(CFLAGS) $(SRC) -o montador-ias
susy:
	@$(CC) $(SUSYCFLAGS) $(SRC) -o montador-ias
profiling:
	@$(CC) $(PROFILINGCFLAGS) $(SRC) -o montador-ias
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
