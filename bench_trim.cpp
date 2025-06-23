#include <benchmark/benchmark.h>
#include <string>
#include <string_view>
#include <cctype>
#include <algorithm>
#include <boost/algorithm/string.hpp> // apt install libboost-all-dev

static const std::string kSample = "   \t  hello world  \n  ";

//
// 方法 1：std::string_view + 手动 trim
//
inline std::string_view trim_view(std::string_view sv) {
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.front())))
        sv.remove_prefix(1);
    while (!sv.empty() && std::isspace(static_cast<unsigned char>(sv.back())))
        sv.remove_suffix(1);
    return sv;
}

static void TrimView(benchmark::State& state) {
    for (auto _ : state) {
        std::string_view trimmed = trim_view(kSample);
        benchmark::DoNotOptimize(trimmed);
    }
}
BENCHMARK(TrimView);

//
// 方法 2：std::string::find_first_not_of + substr
//
static void TrimFindSubstr(benchmark::State& state) {
    for (auto _ : state) {
        size_t start = kSample.find_first_not_of(" \t\n\r\f\v");
        size_t end   = kSample.find_last_not_of(" \t\n\r\f\v");
        std::string trimmed = (start == std::string::npos) ? "" : kSample.substr(start, end - start + 1);
        benchmark::DoNotOptimize(trimmed);
    }
}
BENCHMARK(TrimFindSubstr);

//
// 方法 3：boost::trim_copy
//
static void TrimBoost(benchmark::State& state) {
    for (auto _ : state) {
        std::string trimmed = boost::algorithm::trim_copy(kSample);
        benchmark::DoNotOptimize(trimmed);
    }
}
BENCHMARK(TrimBoost);


BENCHMARK_MAIN();
