#include <thread>
#include <iostream>
#include <future>
#include <string>

std::string TestAsync(int index){
    std::cout << "begin in TestAsync" << index << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return "test async value";  
}

int main(){
    // 创建异步线程
    // 不创建线程启动异步任务
    std::cout << "begin in main" << std::this_thread::get_id() << std::endl;
    auto future = std::async(std::launch::deferred, TestAsync, 100); //延后调用
    std::cout << "begin get future" << std::endl;
    std::cout << "future get()" << future.get() << std::endl; // 调用get的时候进入这个函数
    
    // 创建线程启动异步任务
    std::cout << "begin in main" << std::this_thread::get_id() << std::endl;
    auto future2 = std::async(std::launch::async, TestAsync, 200); //立即调用
    std::cout << "begin get future" << std::endl;
    std::cout << "future get()" << future2.get() << std::endl; // 调用get的时候进入这个函数
    getchar();  
    return 0;
}