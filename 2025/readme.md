
```
class myThread{
public:
    void main(){
        std::cout << "mythread main" << name << std::endl;
    }
private:
    std::string name;
}

myThread th;
th.name = "test mythread";
thread th(&myThread::mian, &myth);
```


#### 线程基类的封装
```

```