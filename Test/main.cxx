#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    // 检查是否有足够的参数
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <param1> <param2> <param3>\n";
        return 1;
    }

    // 将字符串参数转换为浮点数
    float param1 = std::stof(argv[1]);
    float param2 = std::stof(argv[2]);
    float param3 = std::stof(argv[3]);

    // 打印转换后的参数
    std::cout << "参数1: " << param1 << "\n";
    std::cout << "参数2: " << param2 << "\n";
    std::cout << "参数3: " << param3 << "\n";

    return 0;
}
