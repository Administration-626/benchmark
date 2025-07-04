#include <iostream>
#include <vector>
#include <map>
#include <chrono>
#include <random> // 用于生成随机数

// --- 性能测试函数 ---

// 1. switch-case 实现
int switchCaseFunction(int value) {
    switch (value) {
        case 0: return 10;
        case 1: return 20;
        case 2: return 30;
        case 3: return 40;
        case 4: return 50;
        case 5: return 60;
        case 6: return 70;
        case 7: return 80;
        case 8: return 90;
        case 9: return 100;
        // 更多case... 为了测试，可以增加更多case
        case 10: return 110;
        case 11: return 120;
        case 12: return 130;
        case 13: return 140;
        case 14: return 150;
        case 15: return 160;
        case 16: return 170;
        case 17: return 180;
        case 18: return 190;
        case 19: return 200;
        default: return -1; // 默认值
    }
}

// 2. 数组查表实现 (适用于连续小范围整数)
std::vector<int> lookupTableSmall = {
    10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
    110, 120, 130, 140, 150, 160, 170, 180, 190, 200
};

int arrayLookupFunctionSmall(int value) {
    if (value >= 0 && value < lookupTableSmall.size()) {
        return lookupTableSmall[value];
    }
    return -1; // 默认值
}

// 3. std::map 查表实现 (适用于离散或较大范围整数)
std::map<int, int> lookupTableMap;

// 初始化 map
void initMap() {
    for (int i = 0; i < 20; ++i) {
        lookupTableMap[i] = (i + 1) * 10;
    }
    // 添加一些离散的，较大范围的值来模拟实际情况
    lookupTableMap[1000] = 10000;
    lookupTableMap[2000] = 20000;
    lookupTableMap[3000] = 30000;
    lookupTableMap[4000] = 40000;
    lookupTableMap[5000] = 50000;
}

int mapLookupFunction(int value) {
    auto it = lookupTableMap.find(value);
    if (it != lookupTableMap.end()) {
        return it->second;
    }
    return -1; // 默认值
}

// --- 基准测试工具函数 ---

// 测量函数执行时间
template<typename Func>
long long measureTime(Func func, const std::vector<int>& inputs) {
    auto start = std::chrono::high_resolution_clock::now();
    volatile int result = 0; // 使用 volatile 避免编译器优化掉计算
    for (int input : inputs) {
        result += func(input);
    }
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

int main() {
    initMap(); // 初始化 std::map

    const int NUM_ITERATIONS = 1000000; // 迭代次数

    std::random_device rd;
    std::mt19937 gen(rd());

    // --- 场景 1: 小范围连续整数输入 (0-19) ---
    std::cout << "--- 场景 1: 小范围连续整数输入 (0-19) ---" << std::endl;

    // 生成随机输入值
    std::uniform_int_distribution<> distribSmall(0, 19); // 对应 switch 和 small array
    std::vector<int> inputsSmall(NUM_ITERATIONS);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        inputsSmall[i] = distribSmall(gen);
    }

    long long timeSwitchSmall = measureTime(switchCaseFunction, inputsSmall);
    std::cout << "Switch-case (小范围): " << timeSwitchSmall << " ns" << std::endl;

    long long timeArraySmall = measureTime(arrayLookupFunctionSmall, inputsSmall);
    std::cout << "数组查表 (小范围): " << timeArraySmall << " ns" << std::endl;

    long long timeMapSmall = measureTime(mapLookupFunction, inputsSmall);
    std::cout << "Map 查表 (小范围): " << timeMapSmall << " ns" << std::endl;

    std::cout << std::endl;

    // --- 场景 2: 较大范围离散整数输入 (包含 0-19 和 1000, 2000 等) ---
    std::cout << "--- 场景 2: 较大范围离散整数输入 (包含 0-19 和 1000, 2000 等) ---" << std::endl;

    // 生成随机输入值，包含 map 中定义的所有键值
    std::vector<int> distinctKeys;
    for (auto const& [key, val] : lookupTableMap) {
        distinctKeys.push_back(key);
    }
    std::uniform_int_distribution<> distribLarge(0, distinctKeys.size() - 1); // 从 map 的键中随机选择
    std::vector<int> inputsLarge(NUM_ITERATIONS);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        inputsLarge[i] = distinctKeys[distribLarge(gen)];
    }

    // 注意：switch-case 在处理离散大范围值时会非常笨重且效率低，这里仅为了对比
    // 在实际应用中，如果值范围很大且不连续，switch-case 不是一个好的选择
    long long timeSwitchLarge = measureTime(switchCaseFunction, inputsLarge);
    std::cout << "Switch-case (大范围): " << timeSwitchLarge << " ns (注意: 对于大范围离散值，switch-case 通常不适用)" << std::endl;

    // 数组查表在大范围离散值时不可行，因为数组索引需要连续
    // 这里我们依然使用 arrayLookupFunctionSmall 来运行，但结果不具备实际参考价值
    long long timeArrayLarge = measureTime(arrayLookupFunctionSmall, inputsLarge);
    std::cout << "数组查表 (大范围): " << timeArrayLarge << " ns (注意: 数组查表不适用于大范围离散值)" << std::endl;

    long long timeMapLarge = measureTime(mapLookupFunction, inputsLarge);
    std::cout << "Map 查表 (大范围): " << timeMapLarge << " ns" << std::endl;

    std::cout << std::endl;
    std::cout << "注意: 结果可能因编译器、优化级别和运行环境而异。多次运行取平均值可获得更稳定的结果。" << std::endl;

    return 0;
}
