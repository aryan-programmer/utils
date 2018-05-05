// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils\object.h"


int main()
{
	using namespace utils;
	using namespace std;
	object w{ 'l' };
	object d{ 2.0 };
	cout << w.get_type() << endl << w.get<char>() << endl;
	w = 100;
	cout << w.get_type() << endl << w.get<int>() << endl;
	vector<object> y{ 42,"life" };
	cout << d.get_type() << endl;
	auto a = y[ 0 ].get<int>();
	cout << a << endl;
	auto b = y[ 1 ].get<char const(&)[5]>();
	return 0;
}