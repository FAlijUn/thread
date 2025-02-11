// 停止内存泄漏，使用无锁数据结构管理内存
// 当要释放一个节点需要确认其他线程有没有持有这个节点
// 添加节点到可删除列表，没有线程通过pop()访问旧对节点进行回收
#include <atomic>
#include <memory>

template<typename T>
class lock_free_stack{
private:
    std::atomic<unsigned> threads_in_pop_;
    void try_reclaim(node* old_head);
    struct node{
        std::shared_ptr<T> data_;
        node *next;

        node(T const& data):
            data_(std::make_shared<T>(data)){}
    };
public:
    std::shared_ptr<T> pop(){
        ++ threads_in_pop_;
        node* old_head = head.load();
        while(old_head && 
            !head.compare_exchange_weak(old_head, old_head->next));
        std::shared_ptr<T> res;
        if(old_head){
            res.swap(old_head->data_)
        }
        try_reclaim(old_head);
        return res;
    }
};

