a.out : main.o print.o
	gcc main.o print.o
main.o : main.c main.h
	gcc -c main.c
print.o : print.c print.h
	gcc -c print.c
clean :
	rm *.o a.out