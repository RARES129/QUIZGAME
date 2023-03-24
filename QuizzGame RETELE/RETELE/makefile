build: 
	gcc server.c -o server.o -lpthread -lm -lsqlite3
	gcc client.c -o client.o 

clean:
	rm -f *~client.o server.o

client:
	./client.o 127.0.0.1 3000

server:
	./server.o


