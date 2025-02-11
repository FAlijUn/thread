// 使用条件变量构建线程安全队列
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue{
private:
  mutable std::mutex mut_;  // empty 是一个const成员函数传入拷贝构造函数的锁是const的 other是一个const引用
                            // 其他线程可以有这个类型的非const引用对象，如果锁住互斥量是一个可变操作，互斥量对象标记成可变   
  std::queue<T> data_queue_;
  std::condition_variable data_cond_; // 条件变量在多个线程等待同一个事件
                                      // 当线程用来分解工作负载，并且只有一个线程可以对通知作出反应
                                      
public:
  threadsafe_queue(){}
  threadsafe_queue(threadsafe_queue const& other){
    std::lock_guard<std::mutex> lk(other.mut_);
    data_queue_=other.data_queue_;
  }

  void push(T new_value){
    std::lock_guard<std::mutex> lk(mut_);
    data_queue_.push(new_value);
    data_cond_.notify_one();
  }

  void wait_and_pop(T& value){
    std::unique_lock<std::mutex> lk(mut_);
    data_cond_.wait(lk,[this]{return !data_queue_.empty();});
    value=data_queue_.front();
    data_queue_.pop();
  }

  std::shared_ptr<T> wait_and_pop(){
    std::unique_lock<std::mutex> lk(mut_);
    data_cond_.wait(lk,[this]{return !data_queue_.empty();});
    std::shared_ptr<T> res(std::make_shared<T>(data_queue_.front()));
    data_queue_.pop();
    return res;
  }

  bool try_pop(T& value){
    std::lock_guard<std::mutex> lk(mut_);
    if(data_queue_.empty())
      return false;
    value=data_queue_.front();
    data_queue_.pop();
    return true;
  }

  std::shared_ptr<T> try_pop(){
    std::lock_guard<std::mutex> lk(mut_);
    if(data_queue_.empty())
      return std::shared_ptr<T>();
    std::shared_ptr<T> res(std::make_shared<T>(data_queue_.front()));
    data_queue_.pop();
    return res;
  }

  bool empty() const{ 
    std::lock_guard<std::mutex> lk(mut_);
    return data_queue_.empty();
  }
};