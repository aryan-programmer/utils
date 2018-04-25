// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace utils;

int fibonacci( static_array<int , 2u> val )
{
	return val[ 0 ] + val[ 1 ];
}
int tribonacci( utils::static_array<int , 3u> val )
{
	return val[ 0 ] + val[ 1 ] + val[ 2 ];
}

float f( int a ) { return 1 + static_cast<float>( a ); }
int g( std::vector<int> v )
{ return std::accumulate( begin( v ) , end( v ) , 0 ); }
std::vector<int> h( float a )
{ return std::vector<int>( 10 , static_cast<int>( a ) ); }
float n( float a , int b , double c )
{ return static_cast<float>( a + b + c ); }


auto brace_print( char a , char b )
{ return [a , b] ( auto x ) { std::cout << a << x << b << ", "; }; }

int main()
{
	using namespace utils;
	using namespace std;
	// Range iterators
	{
		// Fibonacci sequence
		for ( auto i : make_range<2>( fibonacci , 20 , { 0,1 } ) )
		{
			cout << i << ", ";
		}
		cout << endl;
		// Tribonacci sequence
		for ( auto i : make_range<3>( tribonacci , 20 , { 0,0,1 } ) )
		{
			cout << i << ", ";
		}
		cout << endl;
	}
	// Chained functions
	{
		auto twice( [] ( int i ) { return i * 2; } );
		auto thrice( [] ( int i ) { return i * 3; } );
		auto chained( func_chain( twice , thrice , std::minus<int>{} ) );
		cout << chained( 3 , 1 ) << endl
			 << func_chain( f , g , h , n )( 15.0f , 10 , 10. ) << endl;
	}
	// Multicall functions and for_each_value
	{
		auto f1( brace_print( '(' , ')' ) );
		auto g1( brace_print( '[' , ']' ) );
		auto h1( brace_print( '{' , '}' ) );
		auto nl( [] ( auto ) { std::cout << '\n'; } );
		auto call_fgh( multicall( f1 , g1 , h1 , nl ) );
		// Like:
		//	for(const auto& i:{1 , 2 , 3 , 4 , 5})
		//	{
		//		call_fgh(i);
		//	}
		for_each_value( call_fgh , 1 , 2 , 3 , 4 , 5 );
	}
	// transform_if algorithm
	{
		vector<int> v{ 1, 2, 3, 4, 5, 6 };
		auto even( [] ( int i ) { return i % 2 == 0; } );
		auto twice( [] ( int i ) { return i * 2; } );
		auto console_iter = ostream_iterator<int>( cout , "," );
		transform_if(
			v.begin() , v.end() ,
			ostream_iterator<int>( cout , "," ) , even , twice );
		/*
		transforms each value from the start of the range
		('v.begin()') to the end of the range('v.end()')
		using the functor ('twice') and copy it into the 
		output iterator ('console_iter') if the function
		('even') returns true when the current value is 
		passed to it.
		*/
	}
	return 0;
}