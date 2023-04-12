#include <iostream>
#include <string>
class A{
private:
    int _a;
    std::string _s;
public:
    // 普通构造函数
    A(std::string s, int a) : _a(a), _s(s){
        std::cout << "执行 普通构造函数 函数体" << std::endl;
    }
    // 委托构造函数
    A(int a): A("测试A(int a)", a){
        std::cout << "执行 委托构造函数A(int a) 函数体" << std::endl;
    }
    A(std::string s): A(s, 100){
        std::cout << "执行 委托构造函数A(std::string s) 函数体" << std::endl;
    }
    A(): A("测试A()", 4) { }
    A(std::ostream &out): A(){
        out << "我就是打酱油的\n";
    }
 
    // 打印对象内容
    void display() const{
        std::cout << "---------------------" << std::endl;
        std::cout << "s = " << _s << std::endl;
        std::cout << "a = " << _a << std::endl;
        std::cout << "---------------------" << std::endl;
    }
};
 
int main()
{
    A a("测试A(std::string s, int a)", 1);
    a.display();
 
    A b(2);
    b.display();
 
    A c("测试A(std::string s)");
    c.display();
    
    A d(std::cout);
    d.display();
    return 0;
}