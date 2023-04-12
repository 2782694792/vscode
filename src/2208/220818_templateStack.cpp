#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <stdexcept>

using namespace std;

template <typename T>
class Stack
{
private:
	// 元素
	vector<T> params;
	
public:
	/// @brief 入栈
	/// @param elem 入栈元素 
	void push( T const & elem );

	/// @brief 出栈
	void pop( );

	/// @brief 返回栈顶元素
	/// @return 栈顶元素值
	T top( ) const;

	/// @brief 对栈内进行元素是否为空判断
	/// @return 如果为空则返回 true
	bool empty() const
	{
		return params.empty( );
	}
};

template<typename T>
void Stack<T>::push( T const & elem )
{
	// 追加传入元素的副本
	params.push_back( elem );
}

template<typename T>
void Stack<T>::pop( )
{
	if ( params.empty( ) )
	{
		throw out_of_range( "Stack<>::top(): empty stack" );
	}
	// 返回最后一个元素的副本
	return params.back( );
}

template <typename T>
T Stack<T>::top( ) const
{
	if ( params.empty( ) )
	{
		throw out_of_range( "Stack<>::pop(): empty stack" );
	}
	// 删除最后一个元素
	params.pop_back( );
}

int main( )
{
	try
	{
		Stack<int> intStack;
		Stack<string> stringStack;

		intStack.push( 7 );
		cout << intStack.top() << endl;

		stringStack.push( "Hello" );
		cout << stringStack.top() << endl;
		stringStack.pop();
		stringStack.pop();
	}
	catch ( exception const & ex)
	{
		cerr << "Error: " << ex.what() << endl;
		return -1;
	}
}
