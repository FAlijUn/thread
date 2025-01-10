// 使用 std::atomic_flag 实现自旋互斥锁
#include <atomic>

class spinlock_mutex{
  std::atomic_flag flag_;
public:
  spinlock_mutex():
    flag_(ATOMIC_FLAG_INIT) {}
  
  void lock(){
    while(flag_.test_and_set(std::memory_order_acquire)); 
  }
  // test_and_set(), 检查当前值,并且设置新值
  // std::memory_order_acquire, 当前原子操作是获取操作,
  // 在此操作之前的操作必须在此操作完成之后执行
  // 对于其他线程的操作，在当前线程获取锁之后，将看到锁获取之前的数据

  void unlock(){
    flag_.clear(std::memory_order_release);
    // std::memory_order_release, 当前原子操作是释放操作
    // 确保在解锁之前的所有操作对其他线程可见
  }

};