#include <iostream>
#include <fstream>
#include <unistd.h>

int main() {
    std::cout << "run main" << std::endl;

    std::cout << "pid:" << getpid()<<std::endl;
    

    std::ofstream outfile("output.txt"); // 打开文件用于写入
    outfile << "x: " << 0 << std::endl; // 将 x 的值写入文件
    int x = 0;

    while(true)
    {
        x ++;
        std::cout << "x: " << x << std::endl;
        outfile << "x: " << x << std::endl; // 将 x 的值写入文件
        sleep(1); // 让程序运行得慢一些，便于观察
    }

    outfile.close(); // 关闭文件
    return 0;
}