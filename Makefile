CFLAGS=-std=c11 -Wall -g -static

tcc: tcc.c

test: tcc
	./test.sh

clean:
	rm -f tcc *.0 *~ tmp*

