// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <optional>
#include "utils\coro.hpp"

int main()
{
	using namespace utils;
	using namespace std;
	//++Coroutines
	;
	{
		retainer<size_t> ret{
			[]( yielder<size_t> yield )
		{
			//( retainer_ );
			for(size_t i = 0; i<5; i++)
			{
				UTILS_CORO_YIELD( i );
			}
			UTILS_CORO_BREAK;
		} };
		while(ret!=null)
		{
			cout<<*ret<<endl;
			++ret;
		}
	}
	//int a = 100 , b = 1000;
	//++r;
	//cout<<*r<<endl;
	//cout<<*r<<endl;
	//++a;
	//++b;
	//++r;
	//cout<<*r<<endl;
	//++a;
	//++r;
	//cout<<*r<<endl;
	//++b;
	return 0;
}