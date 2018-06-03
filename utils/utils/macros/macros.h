#pragma once
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
