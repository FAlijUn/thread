// 共享锁 shared_lock
// std::shared_timed_mutex
// 读线程lock_shared()，写线程lock()
// 读线程全部结束后，写线程lock()才能成功

#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <string>

std::shared_timed_mutex g_shared_mutex;

void ThreadRead(int i){
    for(;;){
        g_shared_mutex.lock_shared();
        std::cout << "ThreadRead " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        g_shared_mutex.unlock_shared();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ThreadWrite(int i){
    for(;;){
        g_shared_mutex.lock_shared();
        //读取数据
        g_shared_mutex.unlock_shared();
        g_shared_mutex.lock();
        std::cout << "ThreadWrite " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        g_shared_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(){
    for(int i = 0; i < 5; i++){
        std::thread t(ThreadRead, i);
        t.detach();
    }
    for(int i = 0; i < 5; i++){
        std::thread t(ThreadWrite, i);
        t.detach();
    }

    getchar();
}