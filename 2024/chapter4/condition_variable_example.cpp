// 生产者在线程中向缓冲区添加数据
// 消费者从缓冲区中取出数据
// 消费只有在缓冲区中有数据时才能进行，消费者在缓冲区为空时等待
// 生产者只有在缓冲区未满时才能进行，生产者在缓冲区满时等待
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::queue<int> buffer; // 共享队列 缓冲区
const unsigned int MAX_SIZE=5; // 缓冲区的最大容量
std::mutex mtx; // 互斥锁
std::condition_variable cv; // 条件变量

// 生产者函数
void producer(){
  int produced_item=0;
  while(true){
    // 模拟生产数据过程
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::unique_lock<std::mutex> lock(mtx); // 加锁保护缓冲区

    // 如果缓冲区已满，生产者等待
    while (buffer.size() == MAX_SIZE) {
      std::cout << "Buffer is full, producer is waiting..." << std::endl;
      cv.wait(lock); //等待消费者通知
    }

    // 生产数据并加入缓冲区
    buffer.push(produced_item);
    std::cout << "Produced item: " << produced_item << std::endl;
    produced_item++;

    // 通知消费者线程缓冲区有数据
    cv.notify_all();
  }
}

// 消费者函数
void consumer(){
  while(true){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::unique_lock<std::mutex> lock(mtx); // 加锁保护进程

    //如果缓冲区域为空，消费者等待
    while(buffer.empty()){
      std::cout << "Buffer is empty, consumer is waiting..." << std::endl;
      cv.wait(lock);  // 等待生产者通知
    }

    // 消费数据从缓冲区取出
    int consumed_item = buffer.front();
    buffer.pop();
    std::cout << "Consumed item: " << consumed_item << std::endl;

    // 通知生产者线程缓冲区有空间
    cv.notify_all();
  }
}

int main(){
  std::thread producer_thread(producer); // 创建生产者线程
  std::thread consumer_thread(consumer); // 创建消费者线程

  producer_thread.join(); // 等待生产者线程结束
  consumer_thread.join(); // 等待消费者线程结束

  return 0;
}

// std::lock_guard<std::mutex> 和 std::lock_guard<std::unique_lock> 都是用于管理互斥锁的RAII
// std::lock_guard<std::mutex> 一旦加锁之后在作用于结束时自动解锁
// std::unique_lock<std::mutex> 支持延迟加锁，显示加锁和重新加锁 与条件变量一起使用

/*
std::mutex mtx;

std::mutex mtx;

void flexible_lock(int &counter) {
  std::unique_lock<std::mutex> lock(mtx, std::defer_lock);  // 延迟加锁
  // 执行一些与锁无关的操作
  lock.lock();  // 显式加锁
  ++counter;
  lock.unlock();  // 显式解锁
  // 可以在需要时重新加锁
  lock.lock();
  counter += 2;
}



*/