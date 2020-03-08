d:d.o makethread.o 
	gcc d.o makethread.o -o d -lpthread

d.o:d.c common.h 
	gcc -c d.c

makethread.o:makethread.c common.h
	gcc -c makethread.c 

clean:
	rm  d.o makethread.o 
#$(f): $(f).c
#	gcc $(f).c -o $(f)  -lapue -lpthread -lbsd 

