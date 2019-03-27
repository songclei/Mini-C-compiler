CC=gcc

a.out : lex.yy.o y.tab.o symbol.o print.o
	$(CC) print.o symbol.o lex.yy.o y.tab.o -ll -w

print.o : print.c def.h print.h
	$(CC) -c print.c -w

symbol.o : symbol.c def.h symbol.h
	$(CC) -c symbol.c -w

y.tab.o : y.tab.c def.h main.h
	$(CC) -c y.tab.c -w

lex.yy.o : lex.yy.c y.tab.h def.h 
	$(CC) -c lex.yy.c

y.tab.c	: minc.y.c
	yacc -d minc.y.c 

lex.yy.c : minc.lex.c
	lex minc.lex.c 

clean : 
	rm *.o a.out y.tab.c y.tab.h lex.yy.c 

