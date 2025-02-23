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

void TestLockGuard(int i){
    std::lock_guard<std::mutex> lock(g_mutex);
    while(true){

    }
}

int main(){
    
    getchar();
    return 0;   
}