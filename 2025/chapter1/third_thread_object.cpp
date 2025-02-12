//  对象的传递
#include <thread>
#include <iostream>
#include <string>



class param {
public:
    param(){std::cout << "create param" << std::endl;}
    param(const param& p){
        name = p.name;  
        std::cout << "copy param" << std::endl;
    }
    ~param(){std::cout << "destroy param" << std::endl;}
    void set_name(const std::string& n) {name = n;}
    const std::string& get_name() const {return name;}
private:
    std::string name;        
};

// 对象的传递会调用拷贝构造函数
void thread_main(int p1, float p2, std::string str, param p) {
    std::cout << "thread_main p1: " << p1 << " p2: " << p2 << " str: " << str << " " << p.get_name() <<std::endl;
}

void thread_main_ptr(param* p){
    std::cout << "thread main ptr name: " << p->get_name() << std::endl;
}

void thread_main_ref(param& p){
    std::cout << "thread main ptr name: " << p.get_name() << std::endl;
}

int main(int argc, char* argv[]){

    {   
        // 传递引用
        param p;
        p.set_name("test ref");
        // 由于使用的引用会导致模版函数认为不匹配，因此在传递引用的时候需要加上std::ref
        // 对于模版函数如果有使用需要加一个std::ref
        std::thread th(thread_main_ref, std::ref(p));
        th.join();
    }

    getchar();

    {
        // 传递线程指针
        param p; // 对象的创建
        p.set_name("test ptr trans");
        std::thread th(thread_main_ptr, &p); // 传递的是指针没有对象的所有权
        th.join();
        // th.detach(); // 传递指针，如果将线程分离，可能会导致在主线程中对象被销毁，空悬指针
        
    }
    getchar();
    std::thread th;
    {
        float f1 = 3.14;
        param p;
        p.set_name("test");
        // 所有参数都是通过拷贝
        // 由于是拷贝的，因此在f1销毁之后，子线程中的f1不受影响
        th = std::thread(thread_main, 1, f1, "hello", p); // 创建的时候会调用拷贝构造函数
    }
    th.join();

    return 0;
}