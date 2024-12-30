#include <iostream>
#include <thread> // 多线程支持的头文件

void hello(){ // 线程需要有一个初始函数(intitial function)
  std::cout << "Hello cocurrent world" << std::endl;
}

int main(){
  std::thread t(hello); // 在std::thread中指定函数对象作为初始函数
  t.join(); // 在main中等待与std::thread对象相关联的线程
}
