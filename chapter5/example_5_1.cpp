#include <atomic>
#include <iostream>
#include <thread>

std::atomic<bool> ready(false); // 用于表示队列是不是准备好
std::atomic<int> data(0); //存储队列中的数据

void producer(){
  // 准备数据
  data.store(42, std::memory_order_relaxed); // 使用 memory_order_relaxed 表示该操作没有对其他线程的同步要求
  // 通知消费者数据已经准备好
  ready.store(true, std::memory_order_release); // 当前原子操作之后的所有写入操作在该操作之前对其他线程可见
}

void consumer(){
  while(!ready.load(std::memory_order_acquire)){  // 当前原子操作之前的所有读取操作在该操作之后执行
    std::this_thread::yield();  // 主动让出时间片
  } 
  int value = data.load(std::memory_order_relaxed); // 使用 memory_order_relaxed 表示该操作没有对其他线程的同步要求
  std::cout << "value is " << value << std::endl;
}

int main(){
  std::thread t1(producer);
  std::thread t2(consumer);
  t1.join();
  t2.join();
  return 0;
}