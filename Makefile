all:	server	client

server:	server_datagram.o	reading_a_file.o	moving_in_screen.o	jogo.o
	gcc server_datagram.o reading_a_file.o moving_in_screen.o jogo.o -o server -pthread

client:	client_datagram.o	moving_in_screen.o
	gcc client_datagram.o moving_in_screen.o -o client

server.o:	server_datagram.c
	gcc -c server_datagram.c

reading.o:	reading_a_file.c	reading.h
	gcc -c reading_a_file.c

moving.o:	moving_in_screen.c	moving.h
	gcc -c moving_in_screen.c

jogo.o:	jogo.c	jogo.h
	gcc -c jogo.c

client.o:	client_datagram.c
	gcc -c client_datagram.c

clean:
	rm *.o