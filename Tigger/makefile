CC=g++

a.out : main.o analyse.o print.o symbol.o
	$(CC) main.o analyse.o print.o symbol.o -o a.out -w
main.o : main.c main.h def.h
	$(CC) -c main.c -w
analyse.o : analyse.c analyse.h def.h
	$(CC) -c analyse.c -w
print.o : print.c print.h def.h
	$(CC) -c print.c -w 
symbol.o : symbol.c symbol.h def.h
	$(CC) -c symbol.c -w
clean :
	rm *.o a.out
