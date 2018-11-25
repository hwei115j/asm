all: as.exe

out: as.exe
	./as.exe demo out
as.exe: main.c func.c
	gcc main.c func.c -o as.exe
clean:
	rm -f *.o *.exe* *.gch
	
