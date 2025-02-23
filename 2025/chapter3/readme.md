RAII
利用栈特性自动释放锁
使用局部对象来管理资源，资源获取即初始化
临界区域不能加sleep

// 已经有锁了就不上锁
```
    std::lock_guard<std::mutex> lock(g_mutex, adopt_lock);
```
// 结束的时候释放锁

```
    std::unique_lock<std::mutex> lock(g_mutex);
    lock.unlock(); // 临时释放锁
    lock.lock();
```

```
    // 已经拥有锁 不锁定，退出解锁
    g_mutex.lock()
    std::unique_lock<std::mutex> lock(g_mutex, adopt_lock);
```
```
    // 延后加锁 不拥有 退出不加锁
    std::unique_lock<std::mutex> lock(g_mutex, defer_lock);
    加锁退出栈区解锁
    lock.lock();
```
```
    // 尝试加锁 不阻塞 失败不拥有锁
    std::unique_lock<std::mutex> lock(mutex, try_to_lock);
    if(lock.owns_lock()){
        std::cout << "own lock" << std::endl;
    }else{
        std::cout << "not own lock" << std::endl;
    }
```

生产者消费者模型
- 生产者和消费者共享资源变量list队列
- 生产者生产一个产品，通知消费者消费
- 消费者阻塞等待信号和获取信号后消费产品(取出list队列中的数据)
condition_variable
改变共享变量的线程步骤
- 准备好信号量
    std::condition_variable cv;
- 获得std::mutex
    std::unique_lock lock(mutex);
- 获取锁时进行修改
    msgs_push_back(data);
- 释放锁并通知读取线程
    lock.unlock();
    cv.notify_one(); // 通知一个等待信号线程
    cv.notify_all(); // 通知等待线程

等待信号读取共享变量的线程步骤
- 获取与改变共享变量线程共同的mutex
    std::unique_lock lock(mux);
- wait等待信号通知
    没有lambada表达式
    ```
        解锁lock，并阻塞等待notify_one, notify_all
        cv.wait(lock); // 因为有锁所以只有之歌线程在wait，wait的时候会解锁，并阻塞等待notify_one, 有信号通知了会去获取锁
        接受到通知再次获取锁标注，如果此时mux资源被占用
        msg_.front();
        msgs_.pop_front();
    ```
    lambada
    ```
        cv.wait(lock,[],return !msgs_.empty()) // 返回判断返回false锁会释放，返回的是true，锁不会释放
    ```
有阻塞的时候需要考虑退出的情况
