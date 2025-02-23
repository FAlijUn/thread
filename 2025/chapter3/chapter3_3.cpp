// std::unique_lock
// 支持 临时释放锁 unlock
// 支持 adopt_lock 已经拥有锁，不加锁，出栈区会释放
// 支持 defer_lock 延迟加锁, 不加锁，出栈区不释放
// 支持 try_to_lock 尝试加锁， 尝试获取互斥锁而不阻塞，获取失败退出栈区不会释放，通过owns_lock()函数进行判断
#include <thread>
#include <iostream>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <chrono>

class XMutex{
public:
    XMutex(std::mutex& mutex):mutex_(mutex)
    {
        std::cout << "Lock" <<std::endl;
        mutex_.lock();
    }

    ~XMutex()
    {
        std::cout << "Unlock" <<std::endl;
        mutex_.unlock();
    }
private:
    std::mutex& mutex_;
};

static std::mutex g_mutex; 
void TestMutex(){
    while (true){
        XMutex xmutex(g_mutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

int main(){
    TestMutex();
    getchar();
    return 0;   
}