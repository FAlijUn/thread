#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <memory>
#include <future>
// using 在.cpp文件中使用,不在.h文件中使用

class XTask{
public:
    virtual int Run() = 0;
    std::function<bool()> is_exit = nullptr;
    auto Get_Return(){
        // 阻塞等待set
        return p_.get_future().get();
    }
    void SetValue(int v){
        p_.set_value(v);
    }
private:
    // 用来接受返回值
    std::promise<int> p_;
};


class XThreadPool{
public:
    // 初始化线程池
    void Init(int num){
        std::unique_lock<std::mutex> lock(mux_);
        this->thread_num_ = num;
        std::cout << "Init thread pool with " << num << " threads" << std::endl;
    }

    void Start(){
        std::unique_lock<std::mutex> lock(mux_);
        if(thread_num_ <= 0){
            std::cerr << "Thread pool is not initialized" << std::endl;
            return;
        }

        if(!threads_.empty()){
            std::cerr << "Thread pool is already started" << std::endl;
            return;
        }

        // 启动线程
        for(int i=0; i<thread_num_; i++){
            // auto th = new std::thread(&XThreadPool::Run, this);
            auto th = std::make_shared<std::thread>(&XThreadPool::Run, this); // 出了栈区，引用计数减1，创建的时候引用计数加1
            threads_.push_back(th);
        }
    }

    // 线程池的退出
    void Stop(){
        is_exit_ = true;
        cv_.notify_all(); // 通知所有的等待线程
        for(auto &th : threads_){
            th->join();
        }
        std::unique_lock<std::mutex> lock(mux_);
        threads_.clear();
    }

    // void AddTask(XTask* task){
    void AddTask(std::shared_ptr<XTask> task){
        std::unique_lock<std::mutex> lock(mux_);
        tasks_.push_back(task);
        task->is_exit = [this]{return is_exit();};
        lock.unlock();
        cv_.notify_one(); // 先解锁再通知
    }

    std::shared_ptr<XTask> GetTask(){
        std::unique_lock<std::mutex> lock(mux_);
        if(tasks_.empty()){
            cv_.wait(lock); // 阻塞等待
        }

        if(tasks_.empty()){
            return nullptr;
        }
        if(is_exit()){
            return nullptr;
        }

        auto task = tasks_.front();
        tasks_.pop_front();
        return task;
    }

    // 线程池是否退出
    bool is_exit(){return is_exit_;}
    int task_run_count(){return task_run_count_;}
private:
    // 线程池线程的入口函数
    void Run(){
        std::cout << "Thread " << std::this_thread::get_id() << " running" << std::endl;
        while(!is_exit()){
            auto task = GetTask();
            if(task == nullptr){
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            ++task_run_count_;
            try{
                auto re = task->Run();
                task->SetValue(re);
            }catch(...){
                std::cerr << "Task run error" << std::endl;
            }
            --task_run_count_;
        }   
    }

    int thread_num_ = 0;
    std::mutex mux_;
    // std::vector<std::thread*> threads_; // 存放指针不存放对象，存放对象会导致析构函数调用
    std::vector<std::shared_ptr<std::thread>> threads_;
    // std::list<XTask*> tasks_; // 任务列表
    std::list<std::shared_ptr<XTask>> tasks_;
    std::condition_variable cv_;
    bool is_exit_ = false;
    std::atomic<int> task_run_count_= {0}; // 正在运行的函数数量 
};

// 命令行视频转码工具
// ffmpeg
// 用户输入视频源，输出视频尺寸
 
int main(){
    XThreadPool pool;
    pool.Init(16);
    pool.Start();
    return 0;
}