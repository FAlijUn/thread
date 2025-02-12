// lambda 临时函数作为线程入口函数(匿名函数)
/*
lambda 函数
[捕获列表](参数列表) mutable(可选) 异常属性 -> 返回类型 { 函数体 }
捕获列表：捕获外部变量列表
参数列表：形参列表
*/
#include <thread>
#include <iostream>
#include <string>

class TestLambda {
public:
    void test() {
        std::thread th(
            [this](){std::cout << "Hello, World! " << m_str << std::endl;}
        );
        th.join();
    }
private:
    std::string m_str = "TestLambda";
};

int main(int argc, char* argv[]) {
    std::thread th(
        [](int i){std::cout << "Hello, World! " << i << std::endl;},123
    );
    th.join();
    TestLambda testLambda;
    testLambda.test();
}