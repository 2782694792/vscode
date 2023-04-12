// #include <iostream>

// // 关闭动态断言
// #define NDEBUG
// #include <assert.h>

// // 1、使用断言测试 abort 程序
// #define _EXAM_ASSERT_TEST_
// #include "AssertMacro.h"

// // 2、断点陷阱中断 pause 程序
// #include <cassert>
// #define _ASSERT(x) \
//     if (!(x))      \
//         __asm {int 3};

// int main2(void) {
//     int i = 0;

//     // 1、断言 abort 程序
//     EXAM_ASSERT(i > 2);
//     ASSERT(i > 2, "0");
//     assert(i > 2);

//     // 2、断点陷阱中断程序
//     _ASSERT(i > 2);

//     // 3、静态断言
//     // const int j = 0;
//     // static_assert( j != 22, "i equals to 22" );
//     // static_assert( !std::is_arithmetic<int>::value, "only supports
//     // arithmetictypes" );

//     return 0;
// }
