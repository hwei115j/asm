all: as.exe

as.exe: main.o asfunc.o
	gcc main.o asfunc.o -o as.exe
main.o: main.c ash.h
	gcc -c main.c ash.h
asfunc.o: asfunc.c ash.h
	gcc -c asfunc.c ash.h
clean:
	rm -f *.o *.exe *.gch
	
