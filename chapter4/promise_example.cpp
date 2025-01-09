#include <iostream>
#include <thread>
#include <future>
#include <stdexcept>

// 生产者线程任务
void producer(std::promise<int>& p){
  // 模拟生产者线程工作
  std::this_thread::sleep_for(std::chrono::seconds(2));
  try{
    throw std::runtime_error("Exception from producer");
    p.set_value(42);  // 设置结果
  }catch(...){
    p.set_exception(std::current_exception()); // 如果在生产者线程中发生异常，可以通过set_exception()来设置异常
  }
}

// 消费者线程任务
void consumer(std::future<int>& f){
  try{
    int value = f.get(); // 获取结果  在消费者线程中调用get()来获取结果
    std::cout << "The result of the computation is:" << value << std::endl;
  }catch(const std::exception& e){
    std::cerr << "Consumer caught: " << e.what() << std::endl;
  }
}

int main(){
  // 创建promise对象和future对象
  std::promise<int> promise;
  std::future<int> future = promise.get_future();

  // 创建并启动生产者和消费者线程
  std::thread producer_td(producer, std::ref(promise));
  std::thread consumer_td(consumer, std::ref(future));

  // 等待线程结束
  producer_td.join();
  consumer_td.join();

  return 0;
}