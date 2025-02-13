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

void threadMainMux(int i){
    for(;;){
        mtx.lock();
        std::cout << "Thread: " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    //在unlock之后马上lock，会导致死锁需要sleep一段时间
}

int main(){
    for(int i = 0; i < 3; i++){
        std::thread t(threadMainMux, i);
        t.detach(); 
    }
    getchar();
    return 0;
}