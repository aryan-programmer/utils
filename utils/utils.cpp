// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils\function_addons.h"
struct _null
{
	_null() { std::cout << "ctor" << std::endl; }
	_null( const _null& ) { std::cout << "copy ctor" << std::endl; }
	_null( _null&& ) { std::cout << "move ctor" << std::endl; }
	void operator=( const _null& ) { std::cout << "copy" << std::endl; }
	void operator=( _null&& ) { std::cout << "move" << std::endl; }
};

float f( int a ) { return 1 + static_cast<float>( a ); }
int g( std::vector<int> v )
{ return std::accumulate( begin( v ) , end( v ) , 0 ); }
std::vector<int> h( float a )
{ return std::vector<int>( 10 , static_cast<int>( a ) ); }
float n( float a , int b , double c )
{ return static_cast<float>( a + b + c ); }


auto brace_print( char a , char b )
{
	return [a , b] ( auto&& x )
	{
		std::cout << a << std::forward<decltype( x )>( x ) << b << ", ";
	};
}

int main()
{
	using namespace utils;
	using namespace std;
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
		auto nl( [] ( auto&& ) { std::cout << '\n'; } );
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
			v.begin() , v.end() , console_iter , even , twice );
		cout << '\b';
		cout << ' ' << endl;
		/*
		transforms each value from the start of the range
		('v.begin()') to the end of the range('v.end()')
		using the functor ('twice') and copy it into the
		output iterator ('console_iter') if the function
		('even') returns true when the current value is
		passed to it.
		*/
	}
	{
		vector<wstring> otp;
		auto otp_iter = back_inserter( otp );
		split( L"denmark;`sweden;`india;`;`us" , otp_iter , L";`" );
	}
	{
		trie<> t;
		t.insert_all( "car" , "done" , "try" , "cat" , "trie" , "do" );
		cout << t.contains( "ca" ) << endl;
		cout << t.is_prefix( "ca" ) << endl;
		cout << t.contains( "zebra" ) << endl;
		cout << t.is_prefix( "zebra" ) << endl;
		cout << t.contains( "try" ) << endl;
		cout << t.is_prefix( "try" ) << endl;
		cout << t.contains( "card" ) << endl;
		cout << t.is_prefix( "card" ) << endl;
		auto del_ret = t.remove_word( "do" );
		if ( del_ret )
			cout << del_ret->first << endl;
		del_ret = t.remove_word( "trie" );
		if ( del_ret )
			cout << del_ret->first << endl;
		del_ret = t.remove_word( "ca" );
		if ( del_ret )
			cout << del_ret->first << endl;
	}
	{
		object w{ "Hello" };
		object d{ 2.0 };
		const char** c = obj_cast<const char*>( &w );
		cout << w.type().name() << endl << obj_cast<const char*>( w ) << endl;
		w = 100;
		cout << w.type().name() << endl << obj_cast<int>( w ) << endl;
		vector<object> y{ 42 , static_cast<const char*>( "life" ) };
		cout << d.type().name() << endl;
		auto a = obj_cast<int>( y[ 0 ] );
		cout << a << endl;
		auto b = obj_cast<const char*>( y[ 1 ] );
	}
	{
		object a{ _null() };
		object b = a;
		object c = obj_cast<_null>( b );
		auto y = obj_cast<_null>( std::move( b ) );
		auto z = obj_cast<int>( &c );
	}
	return 0;
}
