#include <iostream>

#include <memory>

using namespace std;

int main(int argc, char const *argv[])
{
    // 唯一的指针，不可复制
    unique_ptr<int> up1(new int(5));
    /* Trying to copy the unique pointer. */
    // unique_ptr<int> up11 = up1;

    unique_ptr<int> up2 = move(up1);
    up2.reset(); // 显式释放内存
    up1.reset(); // 重复释放，但不会导致运行时错误

    up2.reset(new int(11)); // "绑定"动态对象

    up2 = nullptr; // 显式销毁所指对象，变为空指针，等价 up2.reset();

    unique_ptr<int> up5(new int(55));
    int *p = up5.release(); // 释放控制权，不会释放内存
    delete p; // 释放堆区资源

    return 0;
}
