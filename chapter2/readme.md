### 线程管理
* 启动一个线程
* 等待这个线程结束
* 放在后台运行
* 如何给已经启动的线程函数传递参数
* 怎么将一个线程的所有权从当前的std::thread对象移交给另一个
* 确定线程数
* 识别特殊线程

每个程序至少有一个线程 执行main函数的线程，其余函数由各自的入口函数

启动线程归结为构造std::thread对象
std::thread可以用可调用类型构造<callable>
将带有函数调用符的类型实例传入std::thread，替换默认的构造函数

```
class background_task{
public:
  void operator()() const{
    do_something();
    do_something_else();
  }
}
background_task f;
std::thread my_thread(f); //f() 替换默认构造函数
```

```
std::thread my_thread(background_task());
// 声明一个名为my_thread的函数，函数带一个参数
// 这个参数background_task()表示函数指针指向没有参数并返回background_task对象的函数
// 并不是启动一个线程

修改之后:
std::thread my_thread((background_task()));
std::thread my_thread{background_task()};
```

```
// lambda表达式
std::thread my_thread([]{
  do_something();
  do_something_else();
}
)
```

等待线程结束，或者让线程自主运行

std::thread 在对象销毁之前还没有确定是等待线程结束还是让线程自主运行的，程序会调用 std::terminate()

分离线程
  函数已经结束，线程访问局部变量
  ```
  struct func{
    int& i_;
    func(int& i): i_(i) {}
    void operator()()
    {
      for(unsigned j=0; j<100000; ++j){
        do_something(i);
      }
    }
  }
  void oops(){
    int some_local_state=0;
    func my_func(some_local_state);
    std::thread my_thread(my_func);
    my_thread.deatch();
  }
  ```
  函数oops()结束时，新的线程会调用do_something(i)
  由于函数oops()结束, i的生命周期结束,do_something(i)会访问已经销毁的变量
  使用的是引用在销毁之后会导致悬空引用

等待线程完成
  等待线程完成，相关的实例需要join()
  my_thread.deatch()====>my_thread.join()
  灵活的等待使用条件变量和futures
  一个线程只能一次join()，一旦使用join(),std::thread对象就不能加入，当对其使用joinable()返回false

特殊情况
  在线程运行之后产生异常，在join()之前抛出，避免应用被异常终止，无异常使用join()

```
// 确保访问本地状态的线程退出后，函数才结束
struct func{
  int& i_;
  func(int& i): i_(i) {}
  void operator()()
  {
    for(unsigned j=0; j<100000; ++j){
      do_something(i);
    }
  }
}
void f(){
  int some_local_state=0;
  func my_func(some_local_state);
  std::thread t(my_func);
  try{
    do_something_in_current_thread();
  }catch(...){
    t.join();
    throw;
  }
  t.join();
}
```

```
// RAII
class thread_guard{
std::thread& t_;
public:
  explicit thread_guard(std::thread& t):t_(t){}
  ~thread_guard(){
    if(t.joinable()){
      t.join();
    }
  }
  thread_guard(thread_guard const&)=delete;
  thread_guard& operator(thread_guard const&)=delete;
}

struct func;

void f(){
  int some_local_state=0;
  func my_func(some_local_state);
  std::thread t(my_func);
  thread_guard g(t);
  do_something_in_current_thread();
}
```

后台运行 detch() 会让线程在后台运行，主线程不能与之交互，分离线程是守护线程(没有任何用户接口，在后台运行的程序)
这种线程的特点就是长时间运行；线程的生命周期可能会从某一个应用起始到结束，可能会在后台监视文件系统，还有可能对缓存进行清理，亦或对数据结构进行优化
```
std::thread t(do_background_work);
t.detach();
assert(!t.joinable());
```

一个文字处理应用同时编辑多个文档  
每个文字处理窗口处理窗口拥有自己的线程
打开一个新的文档就是启动一个新的线程
```
void edit_documemt(std::string const& filename){
  open_docment_and_display_gui(filename);
  while(!done_editing()){
    user_command cmd = get_user_input();
    if(cmd.type == open_new_document){
      std::string const new_name=get_filename_from_user();
      std::thread t(edit_document, new_name);
      t.detach();
    }else{
      proccess_user_input(cmd);
    }
  }
}
```

向线程函数传递参数
```
void f(int i, std::string const& s);
std::thread t(f,3,"hello");
```

```
void update_data_for_widget(widget_id w,widget_data& data);
void oops_again(widget_id w)
{
  widget_data data;
  std::thread t(update_data_for_widget,w,data); // 2
  display_status();
  t.join();
  process_widget_data(data); // 3
}
```
//  2 传递的是data变量内部拷贝的引用
// process_widget_data将会接收到没有修改的data变量
// ==> std::thread t(update_data_for_widget,w,std::ref(data));


std::bind可以传递一个成员函数指针作为线程函数
```
class X
{
  public:
  void do_lengthy_work();
};
X my_x;
std::thread t(&X::do_lengthy_work,&my_x); // 1
```
// 新线程将my_x.do_lengthy_work()作为线程函数
// my_x的地址作为指针对象提供给函数，对象指针的第一个参数 this

提供参数可以移动，不能拷贝
移动: 原始数据中的数据转移到另一个对象 std::unique_ptr
```
void proccess_big_object(std::unique_ptr<big_object>);
std::unique_ptr<big_object> p(new big_object);
p->perpare_date(43);
std::thread t(proccess_big_object,std::move(p));
```

std::thread 所有权可以在多个实例中互相转移，因为这些实例是可移动(movable)且不可复制(aren't copyable)
在同一时间点，就能保证只关联一个执行线程；同时，也允许程序员能在不同的对象之间转移所有权
