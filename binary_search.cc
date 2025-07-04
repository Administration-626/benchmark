#include <time.h>
#include <iostream>
#include <random>
#include <algorithm>

const int N = 1e6;
#define RANGE N

int a[N] = {0};

void setup()
{
	/*
	使用文特森算法生成伪随机数，
	random_device{}() 获取一个随机数最为随机数种子初始化rng
	*/
	std::mt19937 rng(std::random_device{}());

	std::uniform_int_distribution<int> dist(0, RANGE - 1);

    for (int i = 0; i < N; i++)
        a[i] = dist(rng);
    std::sort(a, a + N, std::greater<int>());
}

long long query()
{
    long long checksum = 0;
    for (int i = 0; i < N; i++) {
        int idx = std::lower_bound(a, a + N, rand()) - a; // 不赋值是否会优化掉这段代码
        checksum += idx;
    }
    return checksum;
}

int main()
{
	setup();
	auto start = clock();
	std::cout << start << std::endl;
	long long checksum = query();

	auto time = float(clock() - start) / CLOCKS_PER_SEC;
	std::cout << time << std::endl;
	std::cout << checksum << std::endl;
}
