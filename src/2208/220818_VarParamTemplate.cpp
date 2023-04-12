#include <iostream>
using namespace std;


// 边界条件，func(T end)、func()
void func()
{
    cout << "end" << endl;
}

// 可变参数的模板函数
template<typename T, class ... Args> // T叫模板参数包
void func(T firstParam, Args... args) // args叫函数参数包
{
    cout << firstParam << " —— " << endl;
    func(args...);
}

int main()
{
    func<int>(10);
    // func<int, int>(10, 20);
    func(10, 20);
    func<char, int>('a', 10);
    func<char, const char*, int>('a', "abc", 250);
    //cout << "先后调用：func('a', "abc", 250)、func("abc", 250)、func(250)、func()" << endl;
    cout << R"( 先后调用：func('a', "abc", 250)、func("abc", 250)、func(250)、func() )" << endl;
    return 0;
}
