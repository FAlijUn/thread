# 线程池
线程的创建和销毁是存在开销的
通过线程池，预先创建好线程，在需要使用的时候在线程池中去取

1. 初始化线程池
    确定线程数量，做好互斥访问
2. 启动所有线程
    std::vector<std::thread*> threads; // 替换成共享指针
    ```
        std::unique_lock<std::mutex> lock(mutex_);
        for(int i = 0; i < thread_num_; i++){
            auto th = new thread(XthreadPool::Run, this);
            threads_.push_back(th);
        }
    ```
3. 准备好任务处理基类和插入任务
    ```
        class XTask{
        public:
            // 执行具体的任务
            virtual int Run() = 0;
        }
    ```
    std::list<XTask*> tasks_;
    插入任务，通知线程池处理
    ```
        std::unique_lock<std::mutex> lock(mutex_);
        tasks_.push_back(task);
        condition_.notify_one();
    ```
4. 获取任务接口
    通过条件变量阻塞等待任务
    ```
    XTaskType XThreadPool::GetTaks(通知){
        std::unique_lock<mutex> lock(mutex_);
        if(tasks_.empty()){
            condition_.wait(lock); // 阻塞等待
        }
        if(is_exit_)
            return nullptr;
        if(tasks_.empty()){
            return nullptr;
        }
        auto task = task_.front();
        tasks_.pop_front();
        return task;
    }
    ```
5. 执行任务线程入口函数
    ```
    void XThreadPool::Run(){
        while(!IsExit()){
            // 获取任务
            auto task = GetTask();
            if(!task) continue;
            try{
                task->Run();
            }catch(...){
                
            }
        }
    }
    ```

使用智能指针在出了栈区之后释放
测试查看空间申请释放的过程，在构造函数拷贝构造以及析构函数中进行打印














