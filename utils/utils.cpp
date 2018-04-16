// utils.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct bird
{
	unsigned id;
	float weight;

	~bird()
	{
		std::cout << "destructing a bird" << std::endl;
	}
};

utils::ptr<bird> make_bird()
{
	return utils::make_ptr_universal<bird>( 2u , 3.9f );
}

utils::ptr<bird> grow_bird( utils::ptr<bird> birdPtr )
{
	birdPtr->weight += 2.f;
	return birdPtr;
}

int main()
{
	using namespace utils;
	using namespace std;
	auto bird1 = make_ptr_universal<bird>( 1u , 5.f );
	bird1 = make_bird();
	cout << "id: " << bird1->id << " weight: " << bird1->weight << endl;
	bird1 = grow_bird( move( bird1 ) );
	cout << "id: " << bird1->id << " weight: " << bird1->weight << endl;
	return 0;
}

