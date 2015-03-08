server_main : server_main.o master.o worker.o server.o handle_task.o handle_frame_sync.o
	g++ -g -o server server_main.o master.o worker.o server.o handle_task.o handle_frame_sync.o ./common/jsoncpp/libjson.a -lpthread -lmysqlclient

server_main.o : server_main.cpp
	g++ -c -g server_main.cpp -o server_main.o -I ./common

master.o : master.cpp master.h
	g++ -c -g master.cpp -o master.o -I ./common

worker.o : worker.cpp worker.h
	g++ -c -g worker.cpp -o worker.o -I ./common

server.o : server.cpp server.h
	g++ -c -g server.cpp -o server.o -I ./common

handle_task.o : handle_task.cpp handle_task.h
	g++ -c -g handle_task.cpp -o handle_task.o -I ./common

handle_frame_sync.o : handle_frame_sync.cpp handle_frame_sync.h
	g++ -c -g handle_frame_sync.cpp -o handle_frame_sync.o -I ./common

clean:
	rm -rf *.o server
