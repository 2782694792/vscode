#pragma once

template <typename T, typename U, typename V>
// 仅当模板实例化，里面的3个 static_assert 才会真正被演算，
class MyTemplate
{
	// 确保模板参数 T 是一个非空类
	static_assert(
		!std:is_empty<T>::value,
		"T should be n non-empty class!"
		);

	// 确保模板参数 U 是一个非空类
	static_assert(
		!std:is_empty<U>::value,
		"U should be n non-empty class!"
		);

	// 确保模板参数 V 是从 std::allocator<T> 直接或间接派生而来，
	// 或者 V 就是 std::allocator<T>
	static_assert(
		std:is_base_of<std::allocator<T>, V>::value,
		"V should inherit from std::allocator<T>"
		);
};

//template class MyTemplate<MyClass, MyEmptyClass, std::allocator<MyClass>>;
