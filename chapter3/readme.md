### 线程间共享数据

数据竞争，并发的修改一个独立的对象
* 对于数据结构和不变量的设计进行修改，修改完的结构必须能够完成一系列不可分割的变化，保证不变量的稳定的状态，无锁编程
* 使用事务(transacting)的方式处理数据结构的更新：所需要的一些数据和读取都存储在事务日志中，将之前的操作合并，再进行提交

使用互斥量保护共享数据：当访问共享数据前使用互斥量将相关的数据锁住，再访问结束之后将数据解锁
std::mutex 创建互斥量
lock() 上锁
unlock() 解锁
std::lock_guard()

传递保护数据的引用
```
  class some_data{
  private:
    int a_;
    std::string b_;
  public:
    void do_something();
  };

  class data_wrapper{
  private:
    some_data data;
    std::mutex m;
  public:
    template<typename Function>
    void proccess_data(Function func){
      std::lock_guard<std::mutex> l(m);
      func(data);
    }
  };

  some_data* unprotected;

  void malicious_function(some_data& protected_data){
    unprotected=&protected_data;
  }

  data_wrapper x;
  void foo(){
    x.proccess_data(malicious_function);
    unprotected->do_something();
  }
```

发现接口内在的条件竞争

避免死锁：
* 避免嵌套锁 需要获取多个锁，使用std::lock
* 避免在持有锁的时候调用用户提供的代码
* 使用固定顺序获取锁
* 使用锁的层次顺序

std::unique_lock()