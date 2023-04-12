#include "iostream"
#include "tuple"

using namespace std;
using std::tuple;

int main( int argc, char** argv ) {
    ///////////////////////// #include <tuple> /////////////////////////
    std::tuple< int, char > tuple1;
    tuple1 = make_pair( 1, '1' );
    std::tuple< int, char > tuple2( tuple1 );
    std::tuple< int, char > tuple3( std::make_tuple( 1, 'b' ) );

    std::tuple< long, char > tuple4( tuple3 );  // 左值方式
    std::tuple< long, char > tuple5( 20, 'l' ); // 右值方式
    std::tuple< long, char > tuple6( std::make_pair( 33, 'l' ) ); // 右值方式

    cout << get< 1 >( tuple2 ) << endl; // 1
    cout << get< 1 >( tuple3 ) << endl; // b
    cout << get< 0 >( tuple5 ) << endl; // 20
    cout << get< 1 >( tuple6 ) << endl; // l
}
