// 递归锁

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

std::recursive_mutex r_mutex; 

void Task1(){
    r_mutex.lock();
    std::cout << "Task1 get the lock" << std::endl;
    r_mutex.unlock();
}

void Task2(){
    r_mutex.lock();
    std::cout << "Task2 get the lock" << std::endl;
    Task1();
    r_mutex.unlock();
}

void TaskMainRec(int i){
    for(;;){
        r_mutex.lock();
        std::cout << i << "get the lock" << std::endl;
        Task1();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        Task2();
        r_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}


int main(){

    for(int i = 0; i < 3; i++){
        std::thread t(TaskMainRec, i);
        t.detach();
    }
    getchar();
}