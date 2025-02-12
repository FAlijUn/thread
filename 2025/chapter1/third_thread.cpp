// 参数传递
#include <thread>
#include <iostream>
#include <string>

void thread_main(int p1, float p2, std::string str) {
    std::cout << "thread_main p1: " << p1 << " p2: " << p2 << " str: " << str << std::endl;
}

int main(int argc, char* argv[]){
    
    std::thread th;
    {
        float f1 = 3.14;
        // 所有参数都是通过拷贝
        // 由于是拷贝的，因此在f1销毁之后，子线程中的f1不受影响
        th = std::thread(thread_main, 1, f1, "hello");
    }
    th.join();

    return 0;
}