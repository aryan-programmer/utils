// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils\ext_meth.h"
#include "utils\macros\ext_meth_singular.h"
#include "utils\ext_meth_generic.h"
#include "utils\macros\macros.h"

EXTENSION_METHOD( constexpr float , square_ext , float flt )
{ return flt * flt; }

EXTENSION_METHOD_DOC_PRE( cube_ext )
// Returns the value times iteself 3 times(flt*flt*flt)
EXTENSION_METHOD_DOC( constexpr float , cube_ext , float flt )
{ return flt * flt * flt; }

class square_class :public utils::object_extension_method<float , float>
{
public:
	// Inherited via object_extension_method
	virtual float operator()( float flt ) override { return flt * flt; }
};

auto square = [] ( float value )
{
	return value * value;
};


int main()
{
	using namespace utils;
	using namespace std;
	auto coll = { 1,2,3,4,5 };
	UTILS_FOREACH( auto& i , coll , { cout << i << endl; } )


	// Extension methods via null structs and 
	// explicit operators for those null structs
	{
		cout << 10 % square_ext % cube_ext << endl;
	}
	// Extension methods via generic operators
	// (the only limitation is that the functions have to be function objects like lambda expressions and closures or must have a function named 'call' and they must take only 1 parameter aka they can't be plain functions they must be closures/lambdas/function objects that take only 1 parameter)
	{
		using namespace utils::extension_methods_call_ops::comma;
		cout << ( 10.f , square ) << endl;
	}
	// Extension methods via classes 
	// inheriting object_extension_method.
	// If a class inherits object_extension_method, then it needs
	// to provide a function with the name 'call' and so an instance
	// of that class can be called using the previous method,
	// (Extension methods via generic operators).
	{
		cout << 10.f % square_class() << endl;
	}
	return 0;
}