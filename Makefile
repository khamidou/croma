OBJS=croma.o lex.yy.o parse.o macro.o errors.o
CFLAGS=-c -g
LDFLAGS=-o croma

croma: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)

croma.o: croma.c
	$(CC) $(CFLAGS) croma.c

macro.o: macro.c macro.h
	$(CC) $(CFLAGS) macro.c

parse.o: parse.c parse.h
	$(CC) $(CFLAGS) parse.c

lex.yy.o: lexer.l
	lex lexer.l
	$(CC) $(CFLAGS) lex.yy.c

errors.o: errors.c errors.h	
	$(CC) $(CFLAGS) errors.c

clean:
	rm *.o
	rm *~
	rm croma

tests: croma
	./croma tests/simple
	./croma tests/test.c