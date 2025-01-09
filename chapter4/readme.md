### 同步并发
一个线程会等待另一个线程中特定的事件的发生，或者等待某一个条件的达成
使用条件变量(condition variable)和期望(futures)实现同步

等待条件达成
std::condition_variable(首选)
std::condition_variable_any
在<condition_variable>头文件中声明
需要有一个互斥量才能一起工作

std::condition_variable仅限于std::mutex一起工作
std::condition_variable_any可以和任何满足最低标准的互斥量一起工作

条件变量在多个线程等待同一个事件
* 运行多个数据实例，处理线程
* 新的数据准备完成，调用notify_one()触发一个正在执行wait()的线程
* 只有等待线程被通知了，所有其他处理线程可能都在处理数据

多个线程等待同一个事件，对于通知需要作出回应
* 共享数据初始化，等待共享数据的更新(定期重新初始化)
* 在线程准备数据的时候会调用notify_all(),而非直接调用notify_one()

等待线程只等待一次，当条件为true，就不会等待条件变量

使用期望(future)等待一次性事件
* 当一个线程需要等待一个特定的一次性事件
* 线程会周期性(较短周期)的等待和检查事件是否触发
* 在检查期间会执行其他任务，直到对应的任务触发，等待的期望会变成就绪

唯一期望： std::future<>
共享期望： std::shared_future<>

std::future<>的实例只能与一个指定的事件相关联
std::shared_ptr<>关联多个事件

带有返回值的后台任务
当任务的结果不着急使用，使用std::async启动一个异步任务 返回一个std::future对象
需要调用这个对象get(), 线程阻塞准备数据，返回结果


std::async是否启动一个线程，或是否在期望里面等待同步任务 
std::launch 在新的线程上执行
std::launch::defered 函数调用被延迟到wait()或get()函数调用时执行
std::launch::async 函数必须在所在的独立线程上执行

将任务包装入一个std::packaged_task<>