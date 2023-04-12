#include <iostream>
#include <memory>
using namespace std;

class Son;

class Father
{
public:
    shared_ptr<Son> son_pr;
    Father()
    {
        cout << __FUNCTION__ << endl;
    }
    ~Father()
    {
        cout << __FUNCTION__ << endl;
    }
};

class Son
{
public:
    shared_ptr<Father> father_;
    Son()
    {
        cout << __FUNCTION__ << endl;
    }
    ~Son()
    {
        cout << __FUNCTION__ << endl;
    }
};

int main(int argc, char const *argv[])
{
    auto son = make_shared<Son>();
    auto father = make_shared<Father>();
    son->father_ = father;
    father->son_pr = son;
    cout << "son: " << son.use_count() << endl;
    cout << "father: " << father.use_count() << endl;
    return 0;
}
/*
Son
Father
son: 2
father: 2
分别执行了构造函数，但没有进行析构函数，出现内存泄漏
*/