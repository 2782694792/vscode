#include <csignal>
#include <iostream>
// #include <unistd.h>

using namespace std;

void signalHandler( int signum ) {
    cout << signum << endl;

    // 清理并关闭
    // 终止程序
    exit( signum );
}

int main( int argc, char const* argv[] ) {
    ///////////////////////// #include <csignal> /////////////////////////
    int i = 0;
    // 注册信号 SIGINT 和信号处理程序
    signal( SIGINT, signalHandler );
    while ( ++i ) {
        cout << "Going to sleep...." << endl;
        if ( i == 3 ) {
            raise( SIGINT ); // 2
        }
    }
    return 0;
}
