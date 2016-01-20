all: client server download

download: download.c
	g++ -g -o download download.c

run:
	 ./download bioresearch.byu.edu 80 /CS360/foo.html


client: client.c
	gcc -o client client.c
server: server.c
	gcc -o server server.c

clean:
	rm client server
