#g++ -c ../worker.cpp -o ./worker.o
#g++ -o test_worker test_worker.cpp worker.o -lpthread

./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
./test_client 127.0.0.1 &
