compile: compClient compServer

compClient: basic_client.o pipe_networking.o
	@gcc -o cl basic_client.o pipe_networking.o

compServer: forking_server.o pipe_networking.o
	@gcc -o srv forking_server.o pipe_networking.o

server: srv
	@./srv

client: cl
	@./cl

basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c

forking_server.o: forking_server.c pipe_networking.h
	@gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	@gcc -c pipe_networking.c

clean:
	rm *.o
	rm *~
