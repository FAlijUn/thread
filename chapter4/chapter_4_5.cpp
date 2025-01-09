// std::packaged_task<> 对一个函数可调用对象，绑定一个期望的结果类型
// 当std::packaged_task<>对象被调用时，它会调用与之相关联的函数对象, 并将期望状态设置为就绪
// 可以用在构建线程池的建筑块
// 将每个子任务分解为独立的子任务，每个子任务包含在一个std::package_task<>对象中
// 调度器处理std::packaged_task<>实例
// 构造std::packaged_task<>实例

// 使用std::packaged_task执行一个图形界面线程
#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;

bool gui_shutdown_message_received();
void get_and_proccess_gui_message();

void gui_thread(){  
  while(!gui_shutdown_message_received()){ // 线程循环接收一条关闭图像界面的指令然后关闭
    get_and_proccess_gui_message();        // 轮询界面消息处理
    std::packaged_task<void()> task;
    {
      std::lock_guard<std::mutex> lk(m);
      if(tasks.empty()){
        continue;
      }
      task = std::move(tasks.front());    //在任务队列中提取一个任务，释放队列上的锁
      tasks.pop_front();
    }
    task();//执行任务
  }
}

std::thread gui_bg_thread(gui_thread);

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f){
  std::packaged_task<void()> task(f);         //创建任务，包含一个没有参数没有返回数值的函数
  std::future<void> res = task.get_future();  //调用get_future()获取与任务相关联的期望
  std::lock_guard<std::mutex> lk(m);
  tasks.push_back(std::move(task));
  return res;
}