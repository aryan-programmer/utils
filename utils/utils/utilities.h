#pragma once

#ifndef __UTILITIES__
#define __UTILITIES__

#include <cstdlib>
#include <ctime>
#include <type_traits>
#include <utility>
namespace utils
{
	using llong = long long;
	using uint = unsigned int;
	using ulong = unsigned long;
	using ullong = unsigned long long;
	using ushort = unsigned short;
	using real = long double;

#pragma region Randomization
	// Initializes the random number genreator with the current time as the seed.
	inline void rand_init() { srand( uint( time( 0 ) ) ); }

	// Returns a random long integer number between min [inclusive] and max [exclusive]
	inline llong rand_long( llong min , llong max ) { return ( ( rand() % ( max - min ) ) + min ); }

	// Returns a random integer number between min [inclusive] and max [exclusive]
	inline int rand_int( int min , int max ) { return ( ( rand() % ( max - min ) ) + min ); }

	// Returns a random element from the array you passed in
	template <typename T , int N> inline T rand_elem( T( &arrayParam )[ N ] )
	{ return arrayParam[ rand_int( 0 , N ) ]; }

	// Shuffles the array you passed in
	// WARNING: Will modify the array you passed in
	template <typename T , int N> void shuffle_array( T( &arrayParam )[ N ] )
	{
		for ( int i = 0; i < N - 1; i++ )
		{
			int randomIndex = rand_int( i , N );
			T tempItem = arrayParam[ randomIndex ];
			arrayParam[ randomIndex ] = arrayParam[ i ];
			arrayParam[ i ] = tempItem;
		}
	}
#pragma endregion

#pragma region Math Functions
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline uint abs( int value ) { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline float abs( float value ) { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline double abs( double value ) { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline real abs( real value ) { return ( value < 0 ? -value : value ); }

	// Repeats the value you passes in between 0 and max.
	uint repeat( int value , uint max )
	{
		if ( value < 0 )return repeat( max + uint( value ) , max );
		else if ( uint( value ) >= max )return value % max;
		else return value;
	}
	llong floor( real valueToFloor )
	{ llong xi = llong(valueToFloor); return valueToFloor < xi ? xi - 1 : xi; }
	int floor( double valueToFloor )
	{ int xi = int(valueToFloor); return valueToFloor < xi ? xi - 1 : xi; }
	int floor( float valueToFloor )
	{ int xi = int( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	llong ceil( real valueToCeil )
	{ llong xi = llong( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	int ceil( double valueToCeil )
	{ int xi = int( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	int ceil( float valueToCeil )
	{ int xi = int( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
#pragma endregion

	constexpr uint defaultLength = 5;
	constexpr uint defaultLengthIncrement = 5;
	constexpr int defaultEndIndex = -1;
}
#endif // !__UTILITIES__
