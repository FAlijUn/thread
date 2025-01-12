#include <atomic>
#include <functional>
#include <thread>
#include <vector>

class thread_pool{
    std::atomic_bool done;
    thread_safe_queue<std::funciton<void()>> work_queue;  // 使用线程安全队列来管理任务队列
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread(){ // 从任务队列上获取任务同时执行这些任务
        while(!done){
            std::function<void()> task;
            if(work_queue.try_pop(task)){
                task()
            }else{
                std::this_thread::yield(); // 如果任务队列上没有任务会调用std::this_thread::yield让线程休息 给与其他线程想任务队列推送任务
            }
        }
    }
public:
    thread_pool():done(false),joiner(threads){
        unsigned const thread_count=std::thread::hardware_concurrency();

        try{
            for(unsigned i=0;i<thread_count;++i){
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
            }
        }catch(...){
            done=true;
            throw;
        }
    }

    ~thread_pool(){
        done =true;
    }

    template<typename FunctionType>
    void submit(FunctionType f){
        work_queue.push(std::function<void()>(f));
    }
};