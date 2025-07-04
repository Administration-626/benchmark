#include <benchmark/benchmark.h>
#include <fstream>
#include <string>

std::string get_process_name() {
    std::ifstream comm("/proc/self/comm");
    std::string name;
    std::getline(comm, name);
    return name;
}


static void proc_self_comm(benchmark::State& state) {
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    std::string name = get_process_name();
    // Make sure the variable is not optimized away by compiler
    benchmark::DoNotOptimize(name);
  }
}
// Register the function as a benchmark
BENCHMARK(proc_self_comm);

#include <sys/prctl.h>

std::string get_process_name_by_prctl() {
    char name[17] = {0}; // 限制为16字节
    prctl(PR_GET_NAME, name, 0, 0, 0);
    return std::string(name);
}


static void use_prctl(benchmark::State& state) {
  // Code before the loop is not measured
  for (auto _ : state) {
    std::string name = get_process_name_by_prctl();
  }
}
BENCHMARK(use_prctl);

BENCHMARK_MAIN();
