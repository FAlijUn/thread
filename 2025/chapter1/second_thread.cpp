#include <thread>
#include <iostream>

bool is_exit = false;

void thread_main() {
    std::cout << "sub thread ID: " << std::this_thread::get_id() << " start" << std::endl;    
    // 子线程休眠3秒 释放CPU资源
    for(int i=0; i<10; i++) {
        std::cout << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(is_exit) {
            break;
        }
    }
    std::cout << "sub thread ID: " << std::this_thread::get_id() << " end" << std::endl;
}

int main(int agrc, char* argv[]) {
    std::cout << "main thread ID: " << std::this_thread::get_id() << " start"<<std::endl;
    // 创建启动线程
    {
        std::thread t(thread_main); // 对象被销毁 子线程还在运行
        t.detach(); // 主线程和子线程同时运行 守护线程
        // 主线程退出之后，子线程还在运行
    }
    std::this_thread::sleep_for(std::chrono::seconds(5));
    is_exit = true;
    std::cout << "main thread ID: " << std::this_thread::get_id() << " end"<<std::endl;
    return 0;
}
// 主线程和子线程同时运行