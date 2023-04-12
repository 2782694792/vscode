#include <map>
#include <vector>
#include <string>

#include <iostream>

using namespace std;

typedef map< string, vector<int> > MAP_SVecInt;

ostream & operator<<( ostream & cout2, vector<int> a )
{
	cout2 << " { ";
	for ( int i = 0; i < a.size( ); ++i )
	{
		cout2 << a[ i ] << " ";
	}
	cout2 << " } " << endl;
	return cout2;
}


int main2( )
{
	vector<int> info;
	int i = 1;
	while (  i != 4 )
	{
		info.push_back( 12 * i * 1.377 );
		i++;
	}

	for ( int j = 0; j < info.size(); j++ )
	{
		cout << info.at( j ) << " " ;
	}

	// peson Name a, { Age 22, height 173.3 ......}
	MAP_SVecInt mapPerson;

	// insert pair
	cout << "\ninsert +++++++++++++++++++++++++++++++++++" << endl;

	//mapPerson.insert( pair < string, vector<int>>( "Jim2", { 1 } ) );
	mapPerson.insert( MAP_SVecInt::value_type( "Jim", { info.at( 0 ), info.at( 1 ) } ) );
	mapPerson.insert( pair < string, vector<int>>( "Jim2", { 1, 0, 231 } ) );
	mapPerson[ "Jim4" ] = { 22, 173 };

	cout << "reverse_itreator +++++++++++++++++++++++++++++++++++" << endl;

	MAP_SVecInt::reverse_iterator it2;
	it2 = mapPerson.rbegin( );
	while ( it2 != mapPerson.rend( ) )
	{
		cout << it2->first << " : " << it2->second << endl;
		it2++;
	}

	cout << "find +++++++++++++++++++++++++++++++++++" << endl;

	auto it3 = mapPerson.find( "Jim2" );
	if ( it3 == mapPerson.end( ) )
	{
		cout << "ERROR: " << it3->first << endl;
	}

	cout << "erase Jim4 +++++++++++++++++++++++++++++++++++" << endl;

	mapPerson.erase( "Jim4" );
	auto it33 = mapPerson.find( "Jim4" );
	if ( it33 != mapPerson.end( ) )
	{
		cout << "ERROR: key<Jim4> not be found " << it33->first << endl;
	}

	cout << "sort 映射中的元素是自动按 key 升序排序，不能 sort  +++++++++++++++++++++++++++++++++++" << endl;

	cout << "value_comp +++++++++++++++++++++++++++++++++++" << endl;

	auto last = *mapPerson.rbegin( );
	auto i4 = mapPerson.begin( );
	do
	{
		cout << i4->first
			<< " = "
			<< i4->second
			<< endl;
	}
	while ( mapPerson.value_comp( )( *i4++, last ) );


	cin.get( );

	return 0;
}

