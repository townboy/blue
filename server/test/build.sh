g++ -c ../worker.cpp -o ./worker.o
g++ -o test_worker test_worker.cpp worker.o -lpthread
