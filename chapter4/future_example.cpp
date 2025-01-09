#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int long_computation(int x){
  std::this_thread::sleep_for(std::chrono::seconds(5)); // 模拟长事件的计算
  return x * x;
}

int main(){
  // 启动一个异步任务
  std::future<int> result = std::async(std::launch::async, long_computation, 21);

  // 进行其他任务
  std::cout << "Doing something else..." << std::endl;

  // 等待异步任务完成并获取结果
  int value = result.get(); // get()会阻塞直到任务完成

  std::cout << "The result of the long computation is:" << value << std::endl;

  return 0;
}

// 异常处理
/*
#include <iostream>
#include <future>
#include <stdexcept>

int may_throw_expection(int x){
  int(x == 0){
    throw std::runtime_error("Invaild argument: 0");
  }
  return x*x;
}

int main(){
  std::future<int> result = std::async(std::lauch::async, may_throw_expection, 0);

  try{
    int value = result.get();
    std::cout << "The result of the computation is:" << value << std::endl;
  }catch(const std::exception& e){
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }

  return 0;
}

std::future<>常用操作
1.get() 阻塞直到任务完成，如果任务发生异常，get()会重新抛出异常
2.vaild() 检查std::future 是否与一个有效的异步任务关联
3.wait() 阻塞直到异步任务完成
4.wait_for()和wait_until()指定等待的最大时间
if(result.wait_for(std::chrono::seconds(1))==std::future_status::timeout){
  std::cout << "Task timed out" << std::endl;
}else{
  std::cout << "Task completed" << std::endl;
}
*/

