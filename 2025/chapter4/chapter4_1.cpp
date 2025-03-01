#include <thread>
#include <iostream>
#include <future>
#include <iostream>

void TestFuture(std::promise<std::string> p){
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "begin set value" << std::endl;
    p.set_value("test future value");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "end set value" << std::endl;
}

int main(){
    // 异步传输变量存储
    std::promise<std::string> p; // move 将右值转换成左值
    auto future = p.get_future();
    std::thread th = std::thread(TestFuture, std::move(p)); 
    std::cout << "begin get future" << std::endl;
    std::cout << "future get()" << future.get() << std::endl;
    std::cout << "end get future" << std::endl;
    th.join();
    // std::cout << "future get()" << future.get() << std::endl;
    getchar();
    return 0;
} 