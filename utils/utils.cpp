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

template<typename T>
std::ostream& operator<<( std::ostream& o , const std::vector<T>& v )
{
	int i_ = 0;
	for ( auto& i : v )
	{
		o << i << ( ( ++i_ != v.size() ) ? ',' : '\0' );
	}
	return o;
}

int main()
{
	using namespace utils;
	using namespace std;
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
