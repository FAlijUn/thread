// scoped_lock
// 多个互斥体的免锁
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <chrono>

static std::mutex g_mutex1;
static std::mutex g_mutex2;

// 会发生死锁
void TestScope1(){
    g_mutex1.lock();
    g_mutex2.lock();
    // std::scoped_lock lock(g_mutex1, g_mutex2)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    g_mutex2.unlock();
    g_mutex1.unlock();
}

void TestScope2(){
    g_mutex2.lock();
    g_mutex1.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    g_mutex1.unlock();
    g_mutex2.unlock();
}

int main(){
    {
        std::thread th(TestScope1);
        th.detach(); 
    }
    {
        std::thread th(TestScope2);
        th.detach();
    }
    getchar();
}