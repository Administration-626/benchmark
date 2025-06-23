dummy: my_benchmark.cpp
	g++ -std=c++17 -O2 my_benchmark.cpp -lbenchmark -lpthread -o my_benchmark

bench_trim: bench_trim.cpp
	g++ -std=c++17 -O2 bench_trim.cpp -lbenchmark -o bench_trim