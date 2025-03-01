#include <thread>
#include <iostream>
#include <future>
#include <string>

std::string TestFuture(int index){
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "begin set value" << index <<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return "test future value";
}

int main(){
    std::packaged_task<std::string(int)> task(TestFuture);
    auto result = task.get_future();
    // task(100);
    std::thread th = std::thread(std::move(task), 200);
    std::cout << "begin get future" << std::endl;
    result.wait_for(std::chrono::seconds(2));
    std::cout << "begin get future" << result.get() <<std::endl; // 阻塞等待结果返回
    th.join();
    getchar();
}