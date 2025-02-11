#include <thread>
#include <iostream>

void thread_main() {
    std::cout << "sub thread ID: " << std::this_thread::get_id() << " start" << std::endl;    
    // 子线程休眠3秒 释放CPU资源
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "sub thread ID: " << std::this_thread::get_id() << " end" << std::endl;
}

int main(int agrc, char* argv[]) {
    std::cout << "main thread ID: " << std::this_thread::get_id() << " start"<<std::endl;
    // 创建启动线程
    std::thread t(thread_main);
    // 阻塞主线程，等待子线程执行完毕
    t.join();
    std::cout << "main thread ID: " << std::this_thread::get_id() << " end"<<std::endl;
    return 0;
}