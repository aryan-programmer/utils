#pragma once
#ifndef __UTILITIES__EXTENSION_METHODS__
#define __UTILITIES__EXTENSION_METHODS__
namespace utils
{
#include <type_traits>
	template<typename TReturn , typename TypeToExtend , typename... TParams>
	class object_extension_method
	{ public:virtual TReturn call( TypeToExtend ) = 0; };

	template< typename TReturn , typename TypeToExtend ,
		typename... TParams> TReturn operator%
		( TypeToExtend value ,
		  object_extension_method<TReturn , TypeToExtend , TParams...>&& ext )
	{
		return std::move( ext.call( std::forward<TypeToExtend>( value ) ) );
	}

	template< typename TReturn , typename TypeToExtend ,
		typename... TParams>TReturn operator%
		( TypeToExtend value ,
		  object_extension_method<TReturn , TypeToExtend , TParams...>& ext )
	{
		return std::move( ext.call( std::forward<TypeToExtend>( value ) ) );
	}
}
#endif // !__UTILITIES__EXTENSION_METHODS__
