// 无锁的线程安全栈
// 先入后出
// 确保一个值安全的添加入栈
// 确保只有一个线程能够索引到给定值
// head指针指向第一个节点
// 添加一个节点
// 1. 创建一个新的节点
// 2. 将新节点的next指针指向当前的head
// 3. 将head指针指向新节点
// 第2步和第3步会产生竞争条件
// 在第3步的时候使用一个原子“比较/交换”保证对head的读取

// 删除数据
// 1. 读取当前的head指针的值
// 2. 读取head->next
// 3. 设置head到head->next
// 4. 通过索引node返回data数据
// 5. 删除索引节点
#include <atomic>

template<typename T>
class lock_free_stack {
private:
    struct node {
        T data;
        node* next;

        node(T const& data_): data(data_) {}
    }

    std::atomic<node*> head;
public:
    void push(T const& data){
        node* const new_node = new node(data);
        new_node->next = head.load();
        while(!head.compare_exchange_weak(new_node->next, new_node));
    }

    void pop(T& result){
        node* old_head=head.load();
        while(!head.compare_exchange_weak(old_head, old_head->next));
        result=old_head->data;
    }
};