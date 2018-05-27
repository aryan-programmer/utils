// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct _null
{
	_null() { std::cout << "ctor" << std::endl; }
	_null( const _null& ) { std::cout << "copy ctor" << std::endl; }
	_null( _null&& ) { std::cout << "move ctor" << std::endl; }
	void operator=( const _null& ) { std::cout << "copy" << std::endl; }
	void operator=( _null&& ) { std::cout << "move" << std::endl; }
};

int main()
{
	using namespace utils;
	using namespace std;
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
