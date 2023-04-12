#include <iostream>

int main( )
{
	using namespace std;

	cout << "cout.put( ) 显示字符的方法";
	cout << "\\a 振铃字符 \\b 退格字符 ";

	cout << "输入(7个数)：_______\b\b\b\b\b\b\b";
	long code;
	cin >> code;

}

// g++ -g .\220806_bondini.cpp -o my_test01
template<typename T>
class test
{
	public:
	void function(T& container)
	{
		m_it = container.begin();
	}
	decltype(T().begin()) m_it;
};