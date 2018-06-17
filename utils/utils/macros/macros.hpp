#pragma once
#ifndef __UTILITIES__MACROS__
#define __UTILITIES__MACROS__
#include <utility>
#include <iterator>
#include <cassert>
#define UTILS_FOREACH(var, coll, block)\
{\
 auto& __cOlL__ = coll;\
  auto __EnD__ = std::end(__cOlL__);\
  for(auto __bEgIn__ = std::begin(__cOlL__);\
		 __bEgIn__ != __EnD__;\
		 ++__bEgIn__)\
	{ var = *__bEgIn__; block;}\
}
#define UTILS_RFOREACH(var, coll, block)\
{\
 auto& __cOlL__ = coll;\
  auto __EnD__ = std::rend(__cOlL__);\
  for(auto __bEgIn__ = std::rbegin(__cOlL__); __bEgIn__ != __EnD__; ++__bEgIn__){ var = *__bEgIn__; block}\
}

#ifdef _MSVC_LANG
#define __CPP_LANG _MSVC_LANG
#else
#define __CPP_LANG __cplusplus
#endif // _MSVC_LANG


#if __CPP_LANG > 201402
#define _HAS_CPP17 1
#define _HAS_CPP14 1
#define _HAS_CPP11 1
#elif __CPP_LANG > 201103
#define _HAS_CPP17 0
#define _HAS_CPP14 1
#define _HAS_CPP11 1
#elif __CPP_LANG > 199711
#define _HAS_CPP17 0
#define _HAS_CPP14 0
#define _HAS_CPP11 1
#else
#define _HAS_CPP17 0
#define _HAS_CPP14 0
#define _HAS_CPP11 0
#endif

#if !_HAS_CPP14
#define UTILS_DELETE_FUCTION_SCOPE private
#define UTILS_DELETE_FUNCTION_DEF(f) ;
#define CPP_14_RELAXED_CONSTEXPR constexpr
#else
#define UTILS_DELETE_FUCTION_SCOPE public
#define UTILS_DELETE_FUNCTION_DEF(f) f = delete;
#define CPP_14_RELAXED_CONSTEXPR
#endif // __CPP_LANG < 201402

#ifdef _DEBUG
#	define ASSERT assert
#else
#	ifdef DEBUG
#		define ASSERT assert
#	else
		define ASSERT(expr)
#	endif // DEBUG
#endif


#define UTILS_DELETE_FUNCTION_DECL(f) f;
#define UTILS_DELETE_FUNCTION(f) UTILS_DELETE_FUNCTION_DEF(f)

#endif // !__UTILITIES__MACROS__
