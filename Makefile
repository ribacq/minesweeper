#Minesweeper Makefile

OBJ = main.o data_struct.o game.o text_ui.o solver.o
CC = gcc
LIBS = -lncurses
CFLAGS = -Wall
OUT = minesweeper.out

all: ${OBJ}
	${CC} ${CFLAGS} ${LIBS} -o ${OUT} ${OBJ}

data_struct.o:	data_struct.h
game.o:		data_struct.h game.h
text_ui.o:	data_struct.h game.h text_ui.h
solver.o:	data_struct.h game.h text_ui.h solver.h
main.o:		data_struct.h game.h text_ui.h solver.h

.PHONY: clean
clean:
	-rm ${OUT} ${OBJ}

