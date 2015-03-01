test_server : test_server.o master.o worker.o server.o
	g++ -o test_server test_server.o master.o worker.o server.o -lpthread

test_server.o : test_server.cpp
	g++ -c test_server.cpp -o test_server.o

master.o : ../master.cpp
	g++ -c ../master.cpp -o master.o

worker.o : ../worker.cpp
	g++ -c ../worker.cpp -o worker.o

server.o : ../server.cpp
	g++ -c ../server.cpp -o server.o

clean:
	rm -rf *.o test_server
