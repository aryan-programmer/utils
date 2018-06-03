#pragma once
#include <utility>
#ifndef __UTILITIES__MACROS__FLAGS__
#define __UTILITIES__MACROS__FLAGS__
#define FLAG_FUNCS_CT(__name__,__integral_type__)\
	constexpr inline __name__ operator|( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__integral_type__>( tptr1 ) | static_cast<__integral_type__>( tptr2 ) );}\
	constexpr inline __name__ operator&( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__integral_type__>( tptr1 ) & static_cast<__integral_type__>( tptr2 ) );}\
	constexpr inline __name__ operator^( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__integral_type__>( tptr1 ) ^ static_cast<__integral_type__>( tptr2 ) );}\
	constexpr inline __name__ operator~( const __name__& tptr1 ){\
		return static_cast<__name__>( ~static_cast<__integral_type__>( tptr1 ) );}\
	constexpr inline __name__& operator|=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 | tptr2 );}\
	constexpr inline __name__& operator&=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 & tptr2 );}\
	constexpr inline __name__& operator^=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 ^ tptr2 );}\

#define FLAG(__name__)\
	enum class __name__; FLAG_FUNCS_CT(__name__,std::underlying_type<__name__>::type)\
	enum class __name__
#endif // !__UTILITIES__FLAG__MACROS__
