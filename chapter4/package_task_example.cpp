// std::packaged_task<>封装可调用对象，并包装成一个任务
// 异步执行，将封装好的任务交给一个线程池或其他线程来执行
// 结果获取，std::future 对象与 std::packaged_task<> 绑定，你可以使用 std::future::get() 来获取任务的结果

#include <iostream>
#include <future>
#include <thread>

// 简单函数模拟任务
int do_work(int x){
  std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟长时间的运算
  return x * x; 
}

int main(){
  // 创建一个std::packaged_task<>对象，封装函数do_work
  std::packaged_task<int(int)> task(do_work);

  // 获取std::future对象，与std::packaged_task<>对象绑定
  std::future<int> result = task.get_future();

  // 创建一个线程，执行任务
  std::thread task_td(std::move(task), 21);

  // 主线程做其他事情，直到任务完成
  std::cout << "Doing something else..." << std::endl;

  // 获取任务的结果
  int value = result.get();
  std::cout << "The result of the computation is:" << value << std::endl;

  // 等待线程结束
  task_td.join();

  return 0;
}