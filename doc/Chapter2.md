

## 1、开线程基本操作

### 方法1：子线程传入函数

C++ 使用thread对象时，join()函数会阻塞主线程，detach()函数不会阻塞主线程同时会脱离主线运行，以下代码

```C++

void myprint(int num = 1){
    cout<<"thead: "<<num<<" has started"<<std::endl;
}
int main(int argc, const char** argv) {
    thread t1(myprint,1);
    cout<<"join begin!"<<endl;
    t1.join();
    cout<<"join end!"<<endl;

    thread t2(myprint,2);
    cout<<"detach begin!"<<endl;
    t2.detach();
    cout<<"detach end!"<<endl;

    cout<<"main end!"<<endl;
    return 0;
}
```

运行结果：

```txt
join begin!thead: 
1 has started
join end!
detach begin!
detach end!
main end!
thead: 2 has started
```

同时需要注意线程thread对象如果不使用join()函数或detach()函数，会报错抛出异常。

### 方法2：子线程传入类对象(类要实现void operator()和复制构造函数)

```C++
#include<thread>
#include <iostream>
#include <unistd.h>
using namespace std;
class TestThread{
private:
    int a;
public:
    TestThread(int& i) : a{i}{
        cout<<"Construct TestThread running"<<endl;
    };
    TestThread(const TestThread& ta) : a{ta.a}{			//实现复制构造函数
        cout<<"Copy Construct TestThread running"<<endl;
    }
    ~TestThread() = default;

    void operator() (){									//实现函数调用，在thread中会调用这个
		cout << "operator() begin" << endl;
        sleep(3);
		cout << "a :" << a << endl;
		cout << "operator() end" << endl;
	}
};

int main(int argc, const char** argv) {
    int nnn = 5;
    TestThread ta(nnn);
    thread ppp(ta);
    ppp.join();
    cout<<"Main process"<<endl;
    return 0;
}
```

运行结果：

```txt
Construct TestThread running
Copy Construct TestThread running
operator() begin
a :5
operator() end
Main process
```

### 方法3：传入lambda表达式

```C++
#include<thread>
#include <unistd.h>
#include<iostream>
int main(int argc, const char** argv) {
    auto mylamthread = []{
        cout<<"lambda start"<<endl;
        sleep(3);
        cout<<"lambda end"<<endl;
    };
    cout<<"main start"<<endl;
    thread th(mylamthread);
    th.join();
    cout<<"main end"<<endl;
    return 0;
 }
```

运行结果

```txt
main start
lambda start
lambda end
main end
```



## 2、join()函数调用时机

通常情况下，我们希望在子线程中进行耗时的操作，同时不阻碍主线程。当子线程中耗时操作执行完毕之后，交给主线程之后，主线程再进行操作。**在析构函数中调用join()，保证耗时操作执行完毕之后，再调用析构。那么接下来的问题就是，耗时线程处理完之后，结果应该如何传递给主线程呢。**

```C++
#include<thread>
#include <unistd.h>
#include<iostream>
using namespace std;

void NotWantToBeBlock(){
    std::cout << "main process"<<endl;
}
class TestThread{
private:
    thread t1;
    int a;
public:
    TestThread() = default;
    ~TestThread(){
        cout<<"~TestThread() begin"<<endl;
        t1.join();
        cout<<"~TestThread() end"<<endl;
    };
    void FucPrintLog(int num){
        cout<<"thread num : "<<num<<" has started!!"<<endl;
        sleep(3);
        cout<<"FucPrintLog finish"<<endl;
    }
    void Init(){
        t1 = thread(&TestThread::FucPrintLog,this,a);
    }
    void Start(){
        cout<<"detach begin!"<<endl;
        t1.detach();
        cout<<"detach end!"<<endl;
    }
    void SetPara(int num){
        a = num;
    }
};

int main(int argc, const char** argv) {
    cout<<"main begin"<<endl;
    TestThread* test = new TestThread();
    test->SetPara(10);
    test->Init();
    //test->Start();
    NotWantToBeBlock();
    delete test;
    cout<<"main end"<<endl;
    return 0;
}
```

运行结果：

```txt
main begin
main process
~TestThread() begin
thread num : 10 has started!!
FucPrintLog finish
~TestThread() end
main end
```

