线程的异步
promise
future
启动线程和获取结果在两个线程中

promise 用来传输的变量,存储变量
    std::promise 提供存储异步通信的数值，再通过其对象创建的std::future异步获取结果
    std::promise 只使用一次 set_value

future 提供访问异步操作结果的机制   
    get() 阻塞等待promise set_value的数值


异步调用函数打包
packaged_task 异步函数调用打包
获取每一次执行的结果，将结果存在一个位置，通过另外一个线程获取这些结果

packaged_task 包装函数作为一个对象，用于异步调用，返回值能够通过std::future对象访问
与bind区别，可以异步调用，函数访问和获取返回值分开调用

async
异步运行一个函数，并返回保有结果的std::future
- launch::deferrend延迟执行，在调用wait和get的时候调用代码函数
- launch::async创建线程
- 返回的线程函数类型是std::future<int>

多核并行计算 c++17
base16 编码












