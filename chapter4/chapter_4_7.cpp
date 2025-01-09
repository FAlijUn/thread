#include <list>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input){
  if(input.empty()){
    return input;
  }
  std::list<T> result;
  result.splice(result.begin(), input, input.begin()); // 从input中取出第一个元素
  T const& pivot = *result.begin(); // 以第一个元素为基准

  auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){return t < pivot;}); // 以基准分割input

  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point); // 小于基准的元素

  auto new_lower(sequential_quick_sort(std::move(lower_part))); // 递归排序小于基准的元素
  auto new_higher(sequential_quick_sort(std::move(input))); // 递归排序大于基准的元素

  result.splice(result.end(), new_higher); // 合并大于基准的元素
  result.splice(result.begin(), new_lower); // 合并小于基准的元素

  return result;
} 

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input){
  if(input.empty()){return input;}
  std::list<T> result;
  result.splice(result.begin(), input, input.begin());
  T const& pivot = *result.begin();

  auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){return t < pivot;}); // 以基准分割input

  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point); // 小于基准的元素

  std::future<std::list<T>> new_lower(std::async(&parallel_quick_sort<T>, std::move(lower_part))); // 递归排序小于基准的元素
  auto new_higher(parallel_quick_sort(std::move(input))); // 递归排序大于基准的元素

  result.splice(result.end(), new_higher); // 合并大于基准的元素
  result.splice(result.begin(), new_lower.get()); // 合并小于基准的元素

  return result;
}