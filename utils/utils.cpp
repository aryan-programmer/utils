// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils\property.h"
#include <functional>

class int_holder
{
	int _i;
public:
	int_holder() :_i( 1 ) , i( _i ) { }
	utils::property<int> i;
};

class int_holder2
{
	int _i;
public:
	int_holder2() :_i( 1 ) , i( _i ) { }
	utils::property<int> i;
};

int main()
{
	using namespace utils;
	using namespace std;
	int_holder h;
	h.i.get()++;
	int& h_i = *h.i;
	h_i++;
	int i = h.i;
	int_holder2 h2;
	h2.i = std::move( h.i );
	return 0;
}