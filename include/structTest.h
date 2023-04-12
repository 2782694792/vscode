#include <string>

using namespace std;
struct test2 // 占56
{
    int       a; // 4
    double    b; // 8    12
    long long c; // 8    16开始24
    string    d; // 28   28开始，56
};

struct test3 // 占48
{
    string    d; // 28
    int       a; // 4			28+4
    double    b; // 8		32+8
    long long c; // 8	40+8
};

struct test4 // 占56
{
    string    d; // 28
    double    b; // 8		32+8
    long long c; // 8	40+8
    int       a; // 4 48+4=52   =》比较最大的数据类型，不是
};

struct test1 // 占56
{
    string    d; // 28
    double    b; // 8
    int       a; // 4		32+4
    long long c; // 8	48+8
};
