#pragma once

#include <stdio.h>
#include <stdlib.h>

// 断言TRUE输出
bool exam_assert(const char* file_name, unsigned int line_no) {
    printf("\n[EXAM]Assert failed: %s, line %u\n", file_name, line_no);
    abort();
}

// 若使用断言测试
#ifdef _EXAM_ASSERT_TEST_
// 若条件成立，则无动作
// 否则报告	( \ ：连接符)
#define EXAM_ASSERT(condition)           \
    if (condition) {                     \
        NULL;                            \
    }                                    \
    else {                               \
        exam_assert(__FILE__, __LINE__); \
    }
// #define EXAM_ASSERT( condition ) ((void) ( !condition ) && exam_assert(
// __FILE__, __LINE__ ))
#else // 若不使用断言测试
#define EXAM_ASSERT(condition) NULL
#endif /* end of ASSERT */

#define ASSERT(condition, msg)                   \
    do {                                         \
        if (!(condition))                        \
            printf("Assert failed! [%s] ", msg); \
    } while (0)
