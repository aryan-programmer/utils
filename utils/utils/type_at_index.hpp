#pragma once

#ifndef __UTILITIES__TYPE_AT_INDEX_
#define __UTILITIES__TYPE_AT_INDEX_
namespace utils
{
	template<size_t i , typename... _Ts> class _type_at_index;
	template<size_t i , typename _T> class _type_at_index<i , _T> { public: using type = _T; };
	template<typename _T , typename... _Ts> class _type_at_index<0 , _T , _Ts...> { public: using type = _T; };
	template<size_t i , typename _T , typename... _Ts>
	class _type_at_index<i , _T , _Ts...> { public: using type = typename _type_at_index<i - 1 , _Ts...>::type; };

	template<size_t i , typename... _Ts>
	using type_at_index = typename _type_at_index<i , _Ts...>::type;
}
#endif // !__UTILITIES__TYPE_AT_INDEX_
