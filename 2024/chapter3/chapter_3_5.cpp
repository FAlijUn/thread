#include <mutex>

class some_big_object {}; // 一个大对象

void swap(some_big_object& lhs, some_big_object& rhs) {
  // 交换两个大对象的实现
} 

class X {
private:
  some_big_object some_detail_;
  std::mutex m_;
public:
  X(some_big_object const& sd): some_detail_(sd) {}
  friend void swap(X& lhs, X& rhs) {
    if (&lhs == &rhs) return;
    std::lock(lhs.m_, rhs.m_); // 为避免死锁，使用std::lock()函数
    std::lock_guard<std::mutex> lock_a(lhs.m_, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(rhs.m_, std::adopt_lock);
    swap(lhs.some_detail_, rhs.some_detail_);
  }
};