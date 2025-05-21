#	creator: Polariusz
#	updated: dmw.
#	
#	la_date: 08-05-2025
#
CC	= gcc
CFLAGS	= -Wall -Wextra -Werror -pedantic
CLIBS	= -lncurses 
SRC	= main.c
OUT	= game
# -lm

all:	$(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CLIBS) $(CLAGS) -o $(OUT)

clean:
	rm -f $(OUT)

phony:  all, clean

