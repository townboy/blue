server_main : server_main.o master.o worker.o server.o handle_task.o
	g++ -o server server_main.o master.o worker.o server.o handle_task.o ./common/jsoncpp/libjson.a -lpthread 

server_main.o : server_main.cpp
	g++ -c server_main.cpp -o server_main.o -I ./common

master.o : master.cpp master.h
	g++ -c master.cpp -o master.o -I ./common

worker.o : worker.cpp worker.h
	g++ -c worker.cpp -o worker.o -I ./common

server.o : server.cpp server.h
	g++ -c server.cpp -o server.o -I ./common

handle_task.o : handle_task.cpp handle_task.h
	g++ -c handle_task.cpp -o handle_task.o -I ./common

clean:
	rm -rf *.o server
