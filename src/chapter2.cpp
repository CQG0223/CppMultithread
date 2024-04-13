/*
    chapter2
*/
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<thread>
#include <unistd.h>
using namespace std;

class TestThread{
private:
    int a;
public:
    TestThread(int& i) : a{i}{
        cout<<"Construct TestThread running"<<endl;
    };
    TestThread(const TestThread& ta) : a{ta.a}{
        cout<<"Copy Construct TestThread running"<<endl;
    }
    ~TestThread() = default;

    void operator() (){
		cout << "operator() begin" << endl;
		cout << "a :" << a << endl;
        sleep(3);
		cout << "operator() end" << endl;
	}
};

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
    // int nnn = 5;
    // TestThread ta(nnn);
    // thread ppp(ta);
    // ppp.join();
    // cout<<"Main process"<<endl;
    // return 0;
}