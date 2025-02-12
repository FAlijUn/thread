#include <thread>
#include <iostream>
#include <string>

// 成员函数需要通过this指针来访问成员变量和成员函数
class myThread{
public:
    // 入口函数
    void main(){
        std::cout << "myThread main name: " << name << " age: " << age << std::endl;
    }
public:
    std::string name;
    int age = 100;
};

class XThread{
public:
    virtual void Start(){ // google c++规范
        is_exit_ = false;
        th_ = std::thread(&XThread::main, this); // 创建线程
    }

    virtual void Stop(){
        is_exit_ = true;
        Wait();
    }

    virtual void Wait(){
        if(th_.joinable()){//线程id号没有了
            th_.join();
        }
    }

    bool is_exit(){
        return is_exit_;
    }

private:
    virtual void main() = 0;
    std::thread th_;
    bool is_exit_ = false;
};

class TestXThread : public XThread{
public:
    void main() override // 确保不会写错
    {
        std::cout << "TestXThread main begin" << std::endl;
        while(!is_exit()){
            std::cout << "." << std::flush; // 屏幕输出会输出到标准输出的缓冲中不会立刻显示，使用std::flush刷新
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "TestXThread main end" << std::endl;
    }
    std::string name;

};

int main(int argc, char* argv[]){
    
    TestXThread testth;
    testth.name = "testxthread name";
    testth.Start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    testth.Stop();
    testth.Wait();
    getchar();
    
    myThread mythread;
    mythread.name = "test";
    mythread.age = 20;
    std::thread th(&myThread::main, &mythread); // 成员函数的指针和当前对象的地址
    th.join();
}