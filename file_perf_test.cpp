#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdio> // For fopen, fclose, remove

// 定义测试文件路径
const std::string TEST_FILE_NAME = "test_performance.txt";
// 定义写入的测试内容
const std::string TEST_CONTENT = "Hello, performance test!\n";
// 定义迭代次数
const int ITERATIONS = 100000; // 增加迭代次数以更明显地看到差异

void testFrequentOpenClose() {
    std::cout << "--- 频繁 fopen/fclose 测试 ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        // 使用 C 风格的文件操作，因为 fopen/fclose 是其核心
        FILE* fp = fopen(TEST_FILE_NAME.c_str(), "a"); // "a" 模式追加写入
        if (fp == nullptr) {
            std::cerr << "错误：无法打开文件 for 频繁模式。" << std::endl;
            return;
        }
        fprintf(fp, "%s", TEST_CONTENT.c_str());
        fclose(fp);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "频繁 fopen/fclose 完成 " << ITERATIONS << " 次操作，耗时: "
              << duration.count() << " 秒" << std::endl;
    std::remove(TEST_FILE_NAME.c_str()); // 清理测试文件
}

void testSingleOpenClose() {
    std::cout << "\n--- 单次 fopen/fclose 测试 (写入多次) ---" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    FILE* fp = fopen(TEST_FILE_NAME.c_str(), "w"); // "w" 模式覆盖写入
    if (fp == nullptr) {
        std::cerr << "错误：无法打开文件 for 单次模式。" << std::endl;
        return;
    }

    for (int i = 0; i < ITERATIONS; ++i) {
        fprintf(fp, "%s", TEST_CONTENT.c_str());
    }
    fclose(fp);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "单次 fopen/fclose (写入 " << ITERATIONS << " 次) 完成，耗时: "
              << duration.count() << " 秒" << std::endl;
    std::remove(TEST_FILE_NAME.c_str()); // 清理测试文件
}

int main() {
    // 确保测试文件不存在，避免旧数据干扰
    std::remove(TEST_FILE_NAME.c_str());

    testFrequentOpenClose();
    testSingleOpenClose();

    return 0;
}
