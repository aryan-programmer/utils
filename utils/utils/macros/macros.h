#pragma once
#include <utility>
#include <iterator>
#define UTILS_FOREACH(var, coll, block)\
{\
  auto __EnD__ = std::end(coll);\
  for(auto __bEgIn__ = std::begin(coll); __bEgIn__ != __EnD__; ++__bEgIn__){ var = *__bEgIn__; block}\
}
#define UTILS_RFOREACH(var, coll, block)\
{\
  auto __EnD__ = std::rend(coll);\
  for(auto __bEgIn__ = std::rbegin(coll); __bEgIn__ != __EnD__; ++__bEgIn__){ var = *__bEgIn__; block}\
}
