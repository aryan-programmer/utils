#pragma once
#ifndef __UTILITIES__OPTIONAL__
#define __UTILITIES__OPTIONAL__
#include <memory>

namespace utils
{
	struct nullopt_t { };
	const nullopt_t nullopt;

	template<typename T>
	class optional :public std::unique_ptr<T>
	{
		std::unique_ptr<T>& GetBase() { return *this; }
	public:
		using base = std::unique_ptr<T>;
		using self = optional<T>;
		constexpr inline optional() noexcept: self{ nullopt } { }
		constexpr inline optional( nullopt_t ) noexcept : base{ } { }
		template<typename T_>
		inline optional( T_&& value ) noexcept( std::is_nothrow_constructible<T , decltype( std::forward<T_>( value ) )>::value ) :
			base{ new T( std::forward<T_>( value ) ) } { }
		inline optional( self&& rhs ) noexcept : base{ std::move( rhs.GetBase() ) } { }
		inline optional( const self& rhs ) noexcept( std::is_nothrow_copy_constructible<T>::value ) :
			base{ rhs ? new T( *rhs ) : nullptr } { }
		inline self& operator=( nullopt_t ) noexcept
		{
			base::reset();
			return *this;
		}
		template<typename T_>
		inline self& operator=( T_&& value ) noexcept( std::is_nothrow_constructible<T , decltype( std::forward<T_>( value ) )>::value )
		{
			base::reset( new T( std::forward<T_>( value ) ) );
			return *this;
		}
		inline self& operator=( self&& rhs ) noexcept
		{
			GetBase() = std::move( rhs.GetBase() );
			return *this;
		}
		inline self& operator=( const self& rhs )noexcept( std::is_nothrow_copy_constructible<T>::value )
		{
			base::reset( rhs ? new T( *rhs ) : nullptr );
			return *this;
		}
		template<typename... Ts>
		inline void emplace( Ts&&... values )noexcept( std::is_nothrow_constructible<T , Ts...>::value )
		{ base::reset( new T( std::forward<Ts>( values )... ) ); }
		template<typename... Ts>
		inline void emplace_braced( Ts&&... values ) noexcept( std::is_nothrow_constructible<T , decltype( std::forward<Ts>( values )... )>::value )
		{ base::reset( new T{ std::forward<Ts>( values )... } ); }

		inline bool operator==( nullopt_t ) const noexcept
		{ return GetBase() == nullptr; }
		inline bool operator!=( nullopt_t ) const noexcept
		{ return GetBase() != nullptr; }

		inline explicit operator bool() noexcept { return GetBase() != nullptr; }
		inline bool has_value() noexcept { return GetBase() != nullptr; }

		~optional() { }
	};
}
namespace std
{
	template<typename T>
	struct hash<utils::optional<T>>
	{
		using argument_type = utils::optional<T>;
		using result_type = size_t;

		size_t operator()( const utils::optional<T>& opt ) noexcept(_Is_nothrow_hashable<typename unique_ptr<T>::pointer>::value)
		{
			return hash<unique_ptr<T>>()( opt );
		}
	};
}
#endif // !__UTILITIES__OPTIONAL__
