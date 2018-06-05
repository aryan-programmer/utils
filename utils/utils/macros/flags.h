#pragma once
#include <utility>
#ifndef __UTILITIES__MACROS__FLAGS__
#define __UTILITIES__MACROS__FLAGS__
#define __UTILS__FLAG_FUNCS_CT__(__name__)\
	constexpr inline __name__ operator|( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__##__name__##_underlying_type__>( tptr1 ) | static_cast<__##__name__##_underlying_type__>( tptr2 ) );}\
	constexpr inline __name__ operator&( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__##__name__##_underlying_type__>( tptr1 ) & static_cast<__##__name__##_underlying_type__>( tptr2 ) );}\
	constexpr inline __name__ operator^( const __name__& tptr1 , const __name__& tptr2 ){\
		return static_cast<__name__>( static_cast<__##__name__##_underlying_type__>( tptr1 ) ^ static_cast<__##__name__##_underlying_type__>( tptr2 ) );}\
	constexpr inline __name__ operator~( const __name__& tptr1 ){\
		return static_cast<__name__>( ~static_cast<__##__name__##_underlying_type__>( tptr1 ) );}\
	constexpr inline __name__& operator|=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 | tptr2 );}\
	constexpr inline __name__& operator&=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 & tptr2 );}\
	constexpr inline __name__& operator^=( __name__& tptr1 , const __name__& tptr2 ){\
		return ( tptr1 = tptr1 ^ tptr2 );}\

#define UTILS_FLAG(__name__)\
	enum class __name__;\
	using __##__name__##_underlying_type__ = typename underlying_type<__##__name__##_underlying_type__>::type;\
	 __UTILS__FLAG_FUNCS_CT__(__name__)\
	enum class __name__

#define UTILS_FLAG_CT(__name__, type)\
	enum class __name__: type;\
	using __##__name__##_underlying_type__ = type;\
	 __UTILS__FLAG_FUNCS_CT__(__name__)\
	enum class __name__: type

#define UTILS_FLAG_DECL(__name__) enum class __name__
#define UTILS_FLAG_DECL_CT(__name__, type) enum class __name__: type
#endif // !__UTILITIES__FLAG__MACROS__
