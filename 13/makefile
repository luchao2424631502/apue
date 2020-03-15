#error=/usr/include/error.c
#a:a.c $(error) daemonize.c already_running.c
#	gcc a.c $(error) daemonize.c already_running.c -o a -lpthread
file=/usr/include/error.c
a:a.o error.o daemonize.o already_running.o 
	gcc a.o error.o daemonize.o already_running.o -o a -lpthread
a.o:a.c
	gcc -c a.c
error.o:$(file)
	gcc -c  $(file)
daemonize.o:daemonize.c
	gcc -c daemonize.c
already_running.o:already_running.c
	gcc -c already_running.c

clean:
	rm a.o error.o daemonize.o already_running.o

