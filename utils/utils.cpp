// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "utils\reflection.hpp"

struct M
{
	UTILS_DECL_AND_REFLECT_VARS(
		(int) a ,
		(int) b ,
		(int) c ,
		(int) d
	)
};

int main()
{
	using namespace utils;
	using namespace std;
	//++Reflection
	{
		M m{ 1,2'0,3'00,4'000 };
		auto rm = utils::mirror<~~m;
		rm[ "a" ].as_ref_unsafe<int>() = 1'0;
		rm[ "b" ].as_ref_unsafe<int>() = 2'00;
		rm[ "c" ].as_ref_unsafe<int>() = 3'000;
		rm[ "d" ].as_ref_unsafe<int>() = 40'000;
		swap( rm[ "a" ] , rm[ "d" ] );
	}

	//++Zip Iterator
	{
		vector<int> n{ 1,2,3,4,5,6,7,8,9 };
		vector<string> s{ "a","b","c","Aryan" };
		vector<int> n2{ 10,20,30,40,50 };

		int i = 0;

		for(auto[ n_i , s_i , n2_i ]:pair_iterator(
			zip( n.begin() , s.begin() , n2.begin() ) ,
			zip( n.end() , s.end() , n2.end() ) ))
		{
			cout<<n_i<<" and "<<s_i<<" and "<<n2_i<<endl;
			i++;
		}
	}
	return 0;
}