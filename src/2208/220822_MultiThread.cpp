#include <iostream>

#include <pthread.h>
#include <cstdlib>

using namespace std;

void *func(void *threadid)
{
    // 对传入的参数进行强制类型转换，由无类型指针转换为整形数
    int tid = (*(int *)threadid);
    cout << "线程 ID:" << tid << endl;
    pthread_exit(threadid);
}

int main(int argc, char **argv)
{
    pthread_t threads[5];
    int indexes[NUM_THREADS]; // 用数组来保存i的值
    int rc;
    int i;
    for (i = 0; i < NUM_THREADS; i++)
    {
        cout << "main() : 创建线程, " << i << endl;
        indexes[i] = i; //先保存i的值
        // 传入的时候必须强制转换为void* 类型，即无类型指针
        rc = pthread_create(&threads[i], NULL,
                            func, (void *)&(indexes[i]));
        if (rc)
        {
            cout << "Error:无法创建线程," << rc << endl;
            exit(-1);
        }
    }
    pthread_exit(NULL);
}