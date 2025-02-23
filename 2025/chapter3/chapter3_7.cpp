// 条件变量
#include <thread>
#include <iostream>
#include <mutex>
#include <list>
#include <string>
#include <sstream>
#include <condition_variable>

std::list<std::string> g_msgs; 
std::mutex g_mutex;
std::condition_variable g_cv;

void ThreadWrite(){
    while(true){
        std::stringstream ss;
        ss << "Write msg" << 1;
        std::unique_lock<std::mutex> lock(g_mutex);
        g_msgs.push_back(ss.str());
        lock.unlock();
        g_cv.notify_one();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void ThreadRead(int i){
    while(true){
        std::unique_lock<std::mutex> lock(g_mutex);
        g_cv.wait(lock, []{return !g_msgs.empty();}); // 解锁，阻塞等待信号
        std::string msg = g_msgs.front();
        g_msgs.pop_front();
        lock.unlock();
        std::cout << "Thread " << i << " read msg: " << msg << std::endl;
    }
}

int main(){
    std::thread th(ThreadWrite);
    th.detach();
    for(int i=0; i<3; i++){
        std::thread th(ThreadRead, i);
        th.detach();
    }
    getchar();
    return 0;
}