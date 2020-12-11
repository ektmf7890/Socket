all : Server Client

Server : Server.c
	gcc -o Server.o Server.c

Client : Client.c
	gcc -o Client.o Client.c

clean :
	rm *.o 

