OBJS=croma.o lex.yy.o
CFLAGS=-c
LDFLAGS=-o croma

croma: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS)

croma.o: croma.c
	$(CC) $(CFLAGS) croma.c

lex.yy.o: lexer.l
	lex lexer.l
	$(CC) $(CFLAGS) lex.yy.c