#include <thread>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <assert.h>
#include <mutex>
using namespace std;

void do_some_work();
class BackgroundTask
{
public:
    void operator()() const
    {
        cout << "BackgroundTask" << endl;
        do_some_work();
    }
};

void create_file()
{
    for (int i = 0; i < 3; i++)
    {

        // cout<<"Wirte File"<<endl;
        ofstream of(to_string(i) + ".txt", std::ios::app);
        this_thread::sleep_for(chrono::milliseconds(500));
        of.close();
    }
}

void update_by_value(std::string word)
{
    word[0] = 'U';
    return;
} // 传值没问题，因为不会影响外面的数据

void update_by_ref(std::string &word)
{
    word[0] = 'U';
    return;
} // 传引用

class File
{
public:
    mutex mtx;
    void write(ofstream &of, std::string word)
    {
        for (int i = 0; i < 10; i++)
        {
            {
                std::lock_guard<mutex> lock(mtx);
                of << word << "\n";
            }
            std::this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
};

int main()
{
    cout << std::thread::hardware_concurrency() << endl; // 机器并发数
    thread myThread(do_some_work);
    BackgroundTask f;
    thread myThread2(f);

    // thread myThread(BackgroundTask())会报错，因为这里相当于声明一个myThread函数，接受一个参数，返回一个thread对象，可以使用多组括号避免这个问题 或者新式初始化,// thread createTask {create_file};
    // // lambda 启动线程, c++ 11
    thread myThread3((BackgroundTask()));
    thread myThread4{BackgroundTask()};
    myThread.join();
    myThread2.join();
    myThread3.join();
    myThread4.join();

    thread lambdaThread([]
                        { cout << "Lambda thread" << endl; });
    lambdaThread.join();

    // 通过std::ref 传递引用
    string word = "Hello world";
    thread u1(update_by_value, word);
    u1.join();
    cout << word << endl;
    thread u2(update_by_ref, std::ref(word));
    u2.join();
    cout << word << endl;
    assert(word != "Hello world");

    // 调用对象的方法
    ofstream of("tmp.txt", ios::app);
    File f1;
    thread w1(&File::write, &f1, std::ref(of), "hello");
    thread w2(&File::write, &f1, std::ref(of), "world");
    cout << "W1 Thread ID" << w1.get_id() << endl; // 线程ID 线程执行时线程ID不同，执行完线程ID相同
    cout << "W2 Thread ID" << w2.get_id() << endl; // 线程ID
    cout << (w1.get_id() == w2.get_id()) << endl;
    w1.join();
    w2.join();
    //多线程写入
    of.close();

    cout << "u1 Thread ID" << u1.get_id() << endl; //

    cout << (u1.get_id() == w2.get_id()) << endl;

    // main exit(3)  会退出所有线程，pthread_exit会等待其他线程运行
    thread dt(create_file);
    dt.detach();
    pthread_exit(0);
}
void do_some_work()
{
    cout << "Do some work" << endl;
}