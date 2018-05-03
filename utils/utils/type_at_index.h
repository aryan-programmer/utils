#pragma once

#ifndef __UTILITIES__TYPE_AT_INDEX_
#define __UTILITIES__TYPE_AT_INDEX_
namespace utils
{
	template<size_t i , typename... _Ts> class type_at_index;
	template<size_t i , typename _T> class type_at_index<i , _T> { public: using T = _T; };
	template<typename _T , typename... _Ts> class type_at_index<0 , _T , _Ts...> { public: using T = _T; };
	template<size_t i , typename _T , typename... _Ts>
	class type_at_index<i , _T , _Ts...> { public: using T = typename type_at_index<i - 1 , _Ts...>::T; };
}
#endif // !__UTILITIES__TYPE_AT_INDEX_
