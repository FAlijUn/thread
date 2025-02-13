// 超时锁应用
// 使用try_lock尝试获取锁，如果长时间获取不到，可能存在死锁，可以使用try_lock进行超时处理
// 一直try_lock会大量占用CPU资源，可以使用sleep进行等待

#include <iostream>
#include <thread>
#include <mutex>
#include <string>

std::timed_mutex t_mutex;

void ThreadMainTime(int i){
    for(;;){
        if(t_mutex.try_lock_for(std::chrono::milliseconds(100))){
            std::cout << i << "try_lock_for timeout" << std::endl;
            continue;
            //多少次获取锁失败记录日志
        }
        std::cout << i << "get the lock" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        t_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

int main(){

    for(int i = 0; i < 3; i++){
        std::thread t(ThreadMainTime, i);
        t.detach();
    }
    getchar();
}