// 使用std::async向函数传递参数
#include <string>
#include <future>

struct X {
  void foo(int, std::string const&);
  std::string bar(std::string const&);
};

X x;

auto f1=std::async(&X::foo,&x,42,"hello"); // 调用p->foo(42,"hello"),p是指向x的指针
auto f2=std::async(&X::bar,x,"goodbye"); // 调用tmpx.bar("goodbye"),tmpx是x的拷贝

struct Y {
  double operator()(double);
};

Y y;

auto f3=std::async(Y(),3.141); // 调用tmpy(3.141),tmpy是Y的一个临时拷贝
auto f4=std::async(std::ref(y),2.718); // 调用y(2.718), 右值

// X baz(X&);
// std::async(baz, std::ref(x)); // 调用baz(x), 右值

class move_only {
public:
  move_only();
  move_only(move_only&&);
  move_only(move_only const&) = delete;
  move_only& operator=(move_only&&);
  move_only& operator=(move_only const&) = delete;

  void operator()();
};

auto f5=std::async(move_only()); // 调用tmp(),tmp是一个临时对象

auto f6=std::async(std::launch::async,Y(),1.2);
// auto f7=std::async(std::launch::deferred,baz,std::ref(x));
// auto f8=std::async(
  // std::launch::deferred | std::launch::async,
      // baz,std::ref(x));
// auto f9=std::async(baz,std::ref(x));
// f7.wait();