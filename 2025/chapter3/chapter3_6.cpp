// 使用互斥锁和list模拟线程通信
// 封装线程基类XThread控制线程的启动和停止
// 模拟消息服务器线程接受字符串消息，模拟处理
// 通过unique_lock和mutex互斥访问list<string>消息队列
// 主线程定时发送信息给子线程

#include <thread>
#include <list>
#include <mutex>
#include <iostream>

class Xthread{
public:
    // 启动线程
    virtual void Start(){
        is_exit_ = false;
        th_ = std::thread(&Xthread::Main, this);
    }
    // 设置线程退出并等待
    virtual void Stop(){
        is_exit_ = true;
        Wait();
    }

    virtual void Wait(){
        if(th_.joinable()){
            return th_.join();
        }
    }
    // 线程是否退出
    bool is_exit(){
        return is_exit_; // 原子变量
    }
private:
    // 线程入口函数
    virtual void Main() = 0;
    bool is_exit_ = false;
    std::thread th_;
};

class XMsgServer : public Xthread{
public:
    // 给当前线程发消息
    void SendMsg(std::string msg){
        std::unique_lock<std::mutex> lock(mutex_);
        msgs_.push_back(msg);
    }
private:
    // 处理消息的线程入口函数
    void Main() override{
        while(!is_exit()){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::unique_lock<std::mutex> lock(mutex_);
            if(msgs_.size() == 0){
                lock.unlock();
                continue;
            }

            while(!msgs_.empty()){
                std::string msg = msgs_.front();
                std::cout << "MsgServer Recv: " << msg << std::endl;
                msgs_.pop_front();
            }
        }
    }
    // 消息队列缓冲
    // 一味的发送会导致消息队列过大，可以设置最大值会将内存耗尽
    std::list<std::string> msgs_;

    std::mutex mutex_; 
};

int main(){
    XMsgServer server;
    server.Start();
    for(int i=0;i<10;i++){
        server.SendMsg("hello");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    server.Stop();
    return 0;   
}
