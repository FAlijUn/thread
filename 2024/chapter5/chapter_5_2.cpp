// 有两个线程
// 一个线程向数据结构中填充数据
// 另一个读取数据结构中的数据
// 第一个线程中设置一个标志，表示数据已经填充完毕
// 第二个线程中检查这个标志，如果数据已经填充完毕，则读取数据

#include <vector>
#include <atomic>
#include <iostream>
#include <thread>

std::vector<int> data;
std::atomic<bool> data_ready(false);

void writer_thread(){
  data.push_back(42);
  data_ready.store(true, std::memory_order_release);
}

void reader_thread(){
  while(!data_ready.load(std::memory_order_acquire)){
    std::this_thread::yield();
  }
  std::cout << "The answer is " << data[0] << std::endl;
}