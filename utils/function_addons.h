#pragma once
#include "utilities.h"
namespace utils
{
#pragma region Function chain
	template<typename T , typename... Ts>
	class func_chain_func :func_chain_func<Ts...>
	{
	public:
		using base = func_chain_func<Ts...>;
		T func;
		template<typename... Args>
		inline func_chain_func( T func , Args&&... funcs ) :func{ func } ,
			base{ std::forward<Args>( funcs )... } { }
		template<typename... Args>
		inline auto operator()( Args&&... args )->
			decltype( func( base::operator()( std::forward<Args>( args )... ) ) )
		{
			return func(
				base::operator()( std::forward<Args>( args )... ) );
		}
	};
	template<typename T>
	class func_chain_func<T>
	{
	public:
		T func;
		inline func_chain_func( T func ) :func{ func } { }
		template<typename... Args>
		inline auto operator()( Args&&... args )->
			decltype( func( std::forward<Args>( args )... ) )
		{
			return func( std::forward<Args>( args )... );
		}
	};
	/* 
	 * Given any list of any functions taking just
	 * one parameter and returning a value that can
	 * be passed to the previous function,
	 * eg:
	 * 1)float f(int),
	 * 2)int g(std::vector<int>), and
	 * 3)std::vector<int> h(float),
	 * and a function whose return value is that first
	 * parameter of the last function from the previous
	 * list of functions and whose parameter types can
	 * be of any type,
	 * eg:float n(float,int,double),
	 * the func chain returns a function object that
	 * mimic calling them in sequence 
	 * eg, like the lambda given below
	 * [&](float a,int b,double c){return f(g(h(n(a,b,c))));}
	 * and if you want to get a functor that does just that 
	 * using a call from this method, then that call would look
	 * like func_chain(f,g,h,n)
	 */
	template< typename... Ts>
	inline func_chain_func<Ts...> func_chain( Ts&&... funcs )
	{ return func_chain_func<Ts...>( std::forward<Ts>( funcs )... ); }
#pragma endregion

#pragma region Multicall functions
	template<typename T , typename... Ts>
	class multicall_func :multicall_func<Ts...>
	{
	public:
		using base = multicall_func<Ts...>;
		T func;
		template<typename... Args>
		inline multicall_func( T func , Args&&... funcs ) :func{ func } ,
			base{ std::forward<Args>( funcs )... } { }
		template<typename... Args>
		inline void operator()( Args&&... args )
		{
			func( args... );
			base::operator()( std::forward<Args>( args )... );
		}
	};
	template<typename T>
	class multicall_func<T>
	{
	public:
		T func;
		inline multicall_func( T func ) :func{ func } { }
		template<typename... Args>
		inline void operator()( Args&&... args )
		{ func( std::forward<Args>( args )... ); }
	};

	template<typename... Funcs>
	inline multicall_func<Funcs...> multicall( Funcs&& ...funcs )
	{
		return multicall_func<Funcs...>( std::forward<Funcs>( funcs )... );
	}
#pragma endregion

#pragma region For each value
	template<typename Func , typename Arg , typename... Args>
	constexpr inline void for_each_value( Func&& func , Arg&& arg , Args&& ...args )
	{
		func( std::forward<Arg>( arg ) );
		for_each_value(
			std::forward<Func>( func ) , std::forward<Args>( args )... );
	}
	template<typename Func , typename Arg>
	constexpr inline void for_each_value( Func&& func , Arg&& arg )
	{
		std::forward<Func>( func )( std::forward<Arg>( arg ) );
	}
#pragma endregion

}