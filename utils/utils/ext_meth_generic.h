#pragma once
#ifndef __UTILITIES__EXTENSION_METHODS__GENERIC__
#define __UTILITIES__EXTENSION_METHODS__GENERIC__
#include <type_traits>
#pragma region __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP
#define __UTILITIES_REM_PAREN(...) __VA_ARGS__
#define __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP_PAREN(op)\
template<typename TFirst , typename TFunc>\
auto inline operator __UTILITIES_REM_PAREN op( TFirst&& callee , TFunc&& func ) -> decltype( std::forward<TFunc>( func )( std::forward<TFirst>( callee ) ) )\
{return std::forward<TFunc>( func )( std::forward<TFirst>( callee ) );}
//template<typename TFirst , typename TFunc>\
//auto inline operator __UTILITIES_REM_PAREN op( TFirst&& callee , TFunc&& func ) -> decltype( std::forward<TFunc>( func ).call\
//	( std::forward<TFirst>( callee ) ) )\
//{return std::forward<TFunc>( func ).call( std::forward<TFirst>( callee ) );}
#define __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP(op) __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP_PAREN((op))
#pragma endregion

namespace utils
{
	namespace extension_methods_call_ops
	{
		namespace modulus { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( % ) }
		namespace bitwise_or { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( | ) }
		namespace bitwise_and { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( &) }
		namespace left_shift { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( << ) }
		namespace less_than { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( < ) }
		namespace less_than_or_eq { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP( <= ) }
		namespace comma { __UTILITIES__DEFINE_EXTENSION_METHOD_CALL_OP_PAREN( ( , ) ) }
	}
}
#endif // !__UTILITIES__EXTENSION_METHODS__GENERIC__

