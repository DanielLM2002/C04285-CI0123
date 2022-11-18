#makefile for Tarea04
cli-tcp.out: cli-tcp.c
		gcc -g cli-tcp.c -o cli-tcp.out

serv-tcp.out: serv-tcp.c
		gcc -g serv-tcp.c -o serv-tcp.out

int-tcp.out: int-tcp.c
		gcc -g int-tcp.c -o int-tcp.out

udp-client.o: udp-client.cc Socket.cc
		g++ -g -c udp-client.cc Socket.cc

udp-server.o: udp-server.cc Socket.cc
		g++ -g -c udp-server.cc Socket.cc

Socket.o: Socket.h Socket.cc
		g++ -g -c Socket.cc

udp-server: udp-server.o Socket.o
		g++ -g udp-server.o Socket.o -o udp-server

udp-client: udp-client.o Socket.o
		g++ -g udp-client.o Socket.o -o udp-client

clean:
		rm cli-tcp.out serv-tcp.out udp-client udp-server