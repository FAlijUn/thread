两个线程访问同一个内存位置，为了避免条件竞争，两个线程需要有一定的执行顺序
* 使用互斥量，当一个互斥量在两个线程同时访问前被锁住，同一时间只有一个线程能够访问对应的位置
* 原子操作同步机制

修改顺序
每一个在c++程序中的对象，都有(由程序中的所有线程对象)确定好的修改顺序
标准原子类型不能赋值和拷贝，支持赋值


内存顺序，确保原子操作执行的顺序
* store操作 memory_order_relaxed, memory_order_release, memory_order_seq_cst
* Load操作 memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_seq_cst
* read_modify_read操作 memory_order_relaxed, memory_order_consume, memory_order_acquire, memory_order_release
  memory_order_acq_rel, memory_order_seq_cst, 

std::atomic_flag 标准原子类型
可以在两个状态间切换 设置和清除
```
  std::automic_flag类型的对象被ATOMIC_FLAG_INIT初始化，初始化为清除
  std::automic_flag f=ATOMIC_FLAG_INIT
```
当标志对象已经初始化，只有三件事：销毁，清除或设置

```
  f.clear(std::memory_order_release); // 释放语义清除标志
  bool x = f.test_and_set(); // 使用默认内存顺序设置并检索旧值
```
赋值和拷贝会破坏原子性

```
  std::atomic<bool> b;
  bool x = b.load(std::memory_order_acquire);  // 加载
  b.store(true);  // 存储操作
  x = b.exchange(false, std::memory_order_acq_rel);  // 使用新选的值替换已存储的值 
  // std::memory_order_acq_rel 获取和释放 读取发生在该原子操作之前，写入发生在该原子操作之后
  // acquire(获取) 确保该原子操作之后的所有操作（读取数据等）不会在该操作之前执行
  // realse(释放) 确保该原子操作之前的所有操作（写入数据等）会在该操作之后执行

```


存储一个新值，取决于当前数值 compare_exchange_weak() 和 compare_exchange_strong()成员函数
比较原子变量的当前值和提供的预期值
* 当两个数值相同的时候存储预期值
* 当两个数值不同预期变量更新成原子变量中的数值
* compare_exchange_weak() 原始值和预期值一致时，存储也可能不成功
```
bool expected = false;
extern atomic<bool> b;
while(!b.compare_exchange_weak(expected, true) && !expected)
```
```
std::atomic<bool> b;
bool expected;
b.compare_exchange_weak(expected, true, 
  memory_order_acq_rel,memory_order_acquire);
b.compare_exchage_weak(expected, true, memory_order_acq_rel);
```

std::atomic<bool>不是无锁的，需要内置一个互斥量

std::atomic<T*>
* fetch_add()
* fetch_sub()
在存储地址上做原子加法和减法 为+=,-=,++,--提供封装
```
  class Foo{};
  Foo some_array[5];
  std::automic<Foo*> p(some_array);
  Foo* x=p.fetch_add(2); // p加2，并返回原始值
  assert(x==some_arry);
  assert(p.load()==&some_array[2]);
  x=(p-=1); //p减1并返回原始值
  assert(x==&some_array[1]);
  assert(p.load()==&some_array[1]);
```

### 原子操作的内存顺序
#### 排序一致队列 std::memory_order_seq_cst
  * 程序中的行为从任意角度，序列的顺序都保持一致
  ```
    #include <atomic>
    #include <thread>
    #include <assert.h>

    std::atomic<bool> x,y;
    std::atomic<int> z;

    void write_x(){
      x.store(true, std::memory_order_seq_cst);
    }

    void write_y(){
      y.store(true, std::memory_order_seq_cst);
    }

    void read_x_then_y{
      while(!x.load(std::memory_order_seq_cst));
      if(y.load(std::memory_order_seq_cst)){
        ++z;
      }
    }

    void read_y_then_x{
      while(!y.load(std::memory_order_seq_cst));
      if(x.load(std::memory_order_seq_cst)){
        ++z;
      }
    }

    int main(){
      x=false;
      y=false;
      z=0;
      std::thread a(write_x);
      std::thread b(write_y);
      std::thread c(read_x_then_y);
      std::thread d(read_y_then_x);
      a.join();
      b.join();
      c.join();
      d.join();
      assert(z.load()!=0);
    }
  ```
#### 自由序列 memory_order_relaxed
  在原子类型的操作以自由序列执行，没有任何同步关系

#### 获取-释放序列
  原子加载 memory_order_acquire
  原子存储 memory_order_release
  两者兼具 memory_order_acq_rel

### 栅栏
```
#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void wirte_x_then_y(){
  x.store(true, std::memory_order_relaxed);
  std::atomic_thread_fence(std::memory_order_release);
  y.store(true, std::memory_order_relaxed);
}

void read_x_then_y(){
  while(!y.load(std::memory_order_relaxed));
  std::atomic_thread_fence(std::memory_order_acquire);
  if(x.load(std::memory_order_relaxed)){
    ++z;
  }
}

int main(){
  x = false;
  y = false;
  z = 0;
  std::thread a(write_x_then_y);
  std::thread b(read_y_then_x);
  a.join();
  b.join();
  assert(z.load()!=0);
}
```