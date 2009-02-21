OBJS=lex.yy.o croma.o
CFLAGS=-c
LDFLAGS=-o croma -lfl

croma: OBJS
	$(CC) $(LDFLAGS) $(OBJS)

croma.o: croma.c
	$(CC) $(CFLAGS) croma.c

lex.yy.o: lexer.lex
	lex lexer.lex
	$(CC) $(CFLAGS) lex.yy.c