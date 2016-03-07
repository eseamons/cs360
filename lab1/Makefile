all: download

download: download.c
	g++ -g -o download download.c

run:
	 ./download bioresearch.byu.edu 80 /CS360/foo.html


run2:
	./download http://ec2-54-218-117-159.us-west-2.compute.amazonaws.com 3000 /test_files/test.html

clean:
	rm download
