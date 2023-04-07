CFLAGS = -Wall 
LDLIBS = -lm
CC=gcc

all: lisp 

clean:
	rm -f lisp

lisp: main_clisp.c clisp.c EVALenv.c
	${CC} main_clisp.c -o $@ ${CFLAGS} ${LDLIBS}

.PHONY: all clean
