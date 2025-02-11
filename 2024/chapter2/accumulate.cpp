#include <algorithm>
#include <functional>
#include <numeric>
#include <thread>
#include <vector>


template<typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);

    if (!length) // 输入范围为空
        return init;

    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread; // 计算线程数
    unsigned long const hardware_threads = std::thread::hardware_concurrency(); // 获取硬件支持的线程数
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads); // 计算实际线程数 2不想在单核机器上运行太多线程
    unsigned long const block_size = length / num_threads; // 计算块大小

    std::vector<T> results(num_threads); // 每个线程的结果
    std::vector<std::thread> threads(num_threads - 1); // 一个线程已经在主线程中了

    Iterator block_start = first; // 每个线程的起始位置
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
      Iterator block_end = block_start;
      std::advance(block_end, block_size); // 计算每个线程的结束位置
      // std::advance(iterator& it, Distance n);
      // it 要移动的迭代器
      // n 移动的距离
      threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
      block_start = block_end;
    }
    accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(), results.end(), init);
}