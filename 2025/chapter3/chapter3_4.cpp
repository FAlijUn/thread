// shared_lock
// c++ 14
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <chrono>


int main(){
    {
        // 共享锁
        static std::shared_timed_mutex g_shared_mutex;
        // 读取锁
        {
            // 调用共享锁
            std::shared_lock<std::shared_timed_mutex> lock(g_shared_mutex);
            // 退出栈区会释放锁
        }
        // 写锁
        {
            // 调用共享锁
            std::unique_lock<std::shared_timed_mutex> lock(g_shared_mutex);
            // 退出栈区会释放锁
        }
    }
}