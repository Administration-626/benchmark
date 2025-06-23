all:
	g++ -std=c++17 -O2 my_benchmark.cpp -lbenchmark -lpthread -o my_benchmark
