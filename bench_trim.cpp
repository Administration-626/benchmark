/**
 * @file bench_trim.cpp
 * @brief 测试字符串 trim 的性能
 * @author gemini
 * @date 2025-06-23
 * @version 1.0
 * @details
 * 该文件使用 Google Benchmark 库来测试不同字符串修剪方法的性能。
 * 三种方法包括：
 * 1. 使用 `std::string_view` 和手动 trim。
 * 2. 使用 `std::string::find_first_not_of` 和 `substr` 
 * 3. 使用 Boost 库的 `boost::algorithm::trim_copy`。
 * @note
 * 需要安装 Google Benchmark 和 Boost 库。
 */

#include <benchmark/benchmark.h>
#include <string>
#include <string_view>
#include <cctype>
#include <boost/algorithm/string.hpp> // install boost

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


static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) start++;

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

static void TrimCustom(benchmark::State& state) {
    for (auto _ : state) {
        std::string trimmed = trim(kSample);
        benchmark::DoNotOptimize(trimmed);
    }
}
BENCHMARK(TrimCustom);


int chushi_rtrim(char *str, const char *charlist)
{
	char	*p;
	int	count = 0;

	if (NULL == str || '\0' == *str)
		return count;

	for (p = str + strlen(str) - 1; p >= str && NULL != strchr(charlist, *p); p--)
	{
		*p = '\0';
		count++;
	}

	return count;
}

/******************************************************************************
 *                                                                            *
 * Purpose: strips characters from beginning of string                        *
 *                                                                            *
 * Parameters: str      - [IN/OUT] string for processing                      *
 *             charlist - [IN]     null terminated list of characters         *
 *                                                                            *
 ******************************************************************************/
void chushi_ltrim(char *str, const char *charlist)
{
	char	*p;

	if (NULL == str || '\0' == *str)
		return;

	for (p = str; '\0' != *p && NULL != strchr(charlist, *p); p++)
		;

	if (p == str)
		return;

	while ('\0' != *p)
		*str++ = *p++;

	*str = '\0';
}

static void TrimChushi(benchmark::State& state) {
    for (auto _ : state) {
        char buffer[256];
        strncpy(buffer, kSample.c_str(), sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0'; // 确保字符串以 null 结尾
        chushi_ltrim(buffer, " \t");
        chushi_rtrim(buffer, " \t\n\r");
        benchmark::DoNotOptimize(buffer);
    }
}
BENCHMARK(TrimChushi);

BENCHMARK_MAIN();
