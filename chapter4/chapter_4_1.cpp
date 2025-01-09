#include<mutex> 
#include<queue>
#include<condition_variable>

struct data_chunk {};
std::mutex some_mutex;
std::queue<data_chunk> data_queue; // 在多个线程间共享的队列
std::condition_variable data_cond;

data_chunk prepare_data() {
  return data_chunk();
}

void data_preparation_thread() { 
  while (true) {
    data_chunk const data = prepare_data(); // 准备数据
    std::lock_guard<std::mutex> lk(some_mutex); // 在数据准备完毕后，获取互斥锁，对共享队列进行操作
    data_queue.push(data);
    data_cond.notify_one(); // 通知数据处理线程(如果有的话)
  }
}

void process(data_chunk const&) {}

bool is_last_chunk(data_chunk const&) {
  return false;
}

void data_processing_thread() { // 数据处理线程
  while (true) {
    std::unique_lock<std::mutex> lk(some_mutex);          // 使用std::unique_lock而不是std::lock_guard
                                                         
    data_cond.wait(lk, []{return !data_queue.empty();});  // 等待数据准备线程通知，传递一个lambda表达式作为第二个参数，等待条件为队列不为空
                                                          // 处理线程接受到通知后，重新获取互斥锁 在条件满足的情况下，从wait()返回并继续持有锁
                                                          // 当条件不满足时，线程将对互斥量解锁，并且阻塞等待
                                                          // 当准备数据的线程调用notify_one()通知条件变量
                                                          // 等待线程苏醒，重新获取互斥锁，并且对条件进行进一步检查
    data_chunk data = data_queue.front();                 
    data_queue.pop();
    lk.unlock();
    process(data);
    if(is_last_chunk(data)) {
      break;
    }
  }
}
