#include <thread>
#include <iostream>
#include <string>
#include <mutex>

static std::mutex mtx;
void testThread(){
    mtx.lock();
    // if(!mtx.try_lock()){
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }
    std::cout << "-----------------------------------------" << std::endl;
    for(int i = 0; i < 10; i++){
        std::cout << "Thread: " << std::this_thread::get_id() << " " << i << std::endl;
    }
    std::cout << "-----------------------------------------" << std::endl;
    // mtx.unlock();
}

int main(){
    for(int i = 0; i < 10; i++){
        std::thread t(testThread);
        t.detach(); 
    }
    getchar();
    return 0;
}