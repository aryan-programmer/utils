#pragma once
#ifndef __UTILITIES__HASHING__
#define __UTILITIES__HASHING__
#include <utility>
#include "utilities.h"
#include "iterator_addons.h"
namespace utils
{
	constexpr size_t hash_combine( size_t seed , size_t hash2 )
	{
		return seed ^ ( hash2 + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 ) );
	}

	template<typename Iterator>
	size_t hash_seq(
		size_t seed1 , size_t seed2 ,
		Iterator begin , const Iterator& end )
	{
		size_t hash_ = seed1;
		for ( ; begin != end; ++begin )
			hash_ *= seed2 + std::hash<typename std::iterator_traits<Iterator>::value_type>()( *begin );
		return hash_;
	}

	template<typename Iterator>
	size_t hash_seq( Iterator begin , const Iterator& end )
	{
		std::hash<typename std::iterator_traits<Iterator>::value_type> hasher;
		size_t hash_ = hasher( *begin );
		++begin;
		for ( ; begin != end; ++begin )
			hash_ = hash_combine( hash_ , hasher( *begin ) );
		return hash_;
	}
}
#endif // !__UTILITIES__HASHING__

