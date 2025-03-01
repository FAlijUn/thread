#include <thread>
#include <iostream>
#include <vector>
#include <chrono>

static const char base16[] = "0123456789ABCDEF";

void Base16Encode(const unsigned char* data, size_t size, unsigned char* out) {
  for (size_t i = 0; i < size; ++i) {
    unsigned char d = data[i];
    char a = base16[d >> 4];
    char b = base16[d & 0x0F];
    out[i * 2] = a;
    out[i * 2 + 1] = b;
  }
}

void Base16EncodeThread(const std::vector<unsigned char>& data, std::vector<unsigned char>& out) {
    int size = data.size();
    int th_count = std::thread::hardware_concurrency(); // 系统支持的核心数

    // 切片数据
    int slice_size = size / th_count;
    if(size < th_count){
        slice_size = 1;
        th_count = size;
    }

    // 准备好线程
    std::vector<std::thread> threads;
    threads.resize(th_count);

    // 任务分配到各个线程
    for(int i=0 ; i < th_count; i++){
        int offset = i * slice_size;
        int count = slice_size;
        // 最后一个线程处理剩余的数据，不一定能填充满
        if(th_count && th_count - 1 == i){
            count = slice_size + size % th_count;
        }
        std::cout << offset << ":" << count << std::endl;
        threads[i] = std::thread(Base16Encode, data.data() + offset, count, out.data());
    }

    // 等待线程结束
    for(int i=0; i<th_count; i++){
        threads[i].join();
    }

}

int main(){
    std::string test_data ="test";
    unsigned char out[1024]={0};
    Base16Encode((const unsigned char*)test_data.c_str(), test_data.size(), out);
    std::cout << out << std::endl;

     // 初始化测试数据
    std::vector<unsigned char> in_data;
    in_data.resize(1024*1024*10); // 10M
    for(int i=0; i<in_data.size(); i++){
        in_data[i] = i%256;
    }
    {
        // 输出空间
        std::vector<unsigned char> out_data;
        out_data.resize(in_data.size()*2);
        auto start = std::chrono::system_clock::now();
        Base16Encode(in_data.data(), in_data.size(), out_data.data());
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "duration: " << duration.count() << "ms" << std::endl;
    }

    {
        // 输出空间
        std::vector<unsigned char> out_data;
        out_data.resize(in_data.size()*2);
        auto start = std::chrono::system_clock::now();
        Base16EncodeThread(in_data, out_data);
        auto end = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "duration: " << duration.count() << "ms" << std::endl;
    }

    getchar();
    return 0;

}