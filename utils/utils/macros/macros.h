#pragma once
#ifndef __UTILITIES__MACROS__
#define __UTILITIES__MACROS__
#include <utility>
#include <iterator>
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
#if _MSVC_LANG < 201402
#define UTILS_DELETE_FUCTION_SCOPE private
#define UTILS_DELETE_FUNCTION_DEF(f) ;
#else
#define UTILS_DELETE_FUCTION_SCOPE public
#define UTILS_DELETE_FUNCTION_DEF(f) f = delete;
#endif // _MSVC_LANG < 201402
#else
#if __cplusplus < 201402
#define UTILS_DELETE_FUCTION_SCOPE private
#define UTILS_DELETE_FUNCTION_DEF(f) ;
#else
#define UTILS_DELETE_FUCTION_SCOPE public
#define UTILS_DELETE_FUNCTION_DEF(f) f = delete;
#endif // __cplusplus < 201402
#endif // _MSVC_LANG
#define UTILS_DELETE_FUNCTION_DECL(f) f;
#define UTILS_DELETE_FUNCTION(f) UTILS_DELETE_FUNCTION_DEF(f)

#endif // !__UTILITIES__MACROS__
