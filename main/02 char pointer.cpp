#include "string.h"
#include "structTest.h"
#include <fstream>
#include <iostream>

using namespace std;

test2 test21 = { 1, 2.2, 333LL, "444" };

int main2() {
    /* 简单数据类型
    int a = 120;
    char * ch = ( char * ) ( &a );
    std::cout << "char * :" << *ch << std::endl;
    std::cout << "char :" << ch << std::endl;
    int d = *( int * ) ( ch );
    std::cout << d << std::endl;*/

    /* 复合类型
    char * ch2 = ( char * ) ( &test21 );
    test2 t2 = *( test2 * ) ( ch2 );
    cout << t2.a << endl;
    cout << t2.b << endl;
    cout << t2.c << endl;
    cout << t2.d << endl;*/

    /* 二进制文件 */
    // 文件写入
    ofstream outFile( "1.txt", ios::binary | ios::out );
    outFile.write( ( char* )( &test21 ), sizeof( test2 ) );
    outFile.close();

    // 文件读出，转换内容
    ifstream inFile;
    inFile.open( "1.txt", ios::binary | ios::in );

    //	赋值取出内容
    // char * ch1 = new char( sizeof( test2 ) );

    cout << sizeof( test2 ) << endl; // 56
    cout << sizeof( test3 ) << endl; // 48
    cout << sizeof( test4 ) << endl; // 56
    cout << sizeof( test1 ) << endl; // 56

    char* ch = new char[ sizeof( test2 ) ];
    memset( ch, -1, sizeof( test2 ) );

    if ( inFile.is_open() ) {
        inFile.read( ch, sizeof( test2 ) );
        test2 test22 = *( test2* )ch;
        cout << "_1: " << test22.a << "_2: " << test22.b << "_3: " << test22.c
             << "_4: " << test22.d;
    }
    inFile.close();

    return 0;
}
