#pragma once
#ifndef __UTILITIES__
#define __UTILITIES__
#include <random>
#include <chrono>
#include <type_traits>
#include <utility>
namespace utils
{
	template<typename T1 , typename... Ts>struct and_t
	{ enum { value = T1::value && and_t<Ts...>::value }; };
	template<typename T1 , typename... Ts>struct or_t
	{ enum { value = T1::value || or_t<Ts...>::value }; };
	template<typename T1 , typename T2>struct and_t<T1 , T2>
	{ enum { value = T1::value && T2::value }; };
	template<typename T1 , typename T2>struct or_t<T1 , T2>
	{ enum { value = T1::value || T2::value }; };
	template<typename T> struct not_t { enum { value = !T::value }; };

	template<typename T> struct _rem_ref { using type = T; };
	template<typename T> struct _rem_ref<T&> { using type = T; };
	template<typename T> struct _rem_ref<T&&> { using type = T; };
	template<typename T> using rem_ref = typename _rem_ref<T>::type;

	template<typename T> struct _rem_const { using type = T; };
	template<typename T> struct _rem_const<const T> { using type = T; };
	template<typename T> using rem_const = typename _rem_const<T>::type;

	template<typename T> struct _rem_volatile { using type = T; };
	template<typename T> struct _rem_volatile<volatile T> { using type = T; };
	template<typename T> using rem_volatile = typename _rem_volatile<T>::type;

	template<typename T> using rem_cv = rem_const<rem_volatile<T>>;
	template<typename T> using rem_cvr = rem_ref<rem_cv<T>>;
	template<typename T> using rem_cr = rem_ref<rem_const<T>>;

#pragma region disable_if and select_t
	template<bool , typename = int>struct _disable_if { };
	template<typename T>struct _disable_if<false , T> { using type = T; };
	template<typename Test , class T = int>
	using disable_if = typename _disable_if<Test::value , T>::type;

	template<bool , typename T1 , typename T2>struct _select_t
	{ using type = T1; };
	template<typename T1 , typename T2>struct _select_t<false , T1 , T2>
	{ using type = T2; };
	template<typename Test , typename T1 , typename T2>
	using select_t = typename _select_t<Test::value , T1 , T2>::type;

	template<bool _value>
	using val_to_t = std::bool_constant<_value>;
#pragma endregion


	template<typename T1 , typename T2>struct are_same :std::false_type { };
	template<typename T>struct are_same<T , T> :std::true_type { };

	template<typename T>struct is_lval_ref :std::false_type { };
	template<typename T>struct is_lval_ref<T&> :std::true_type { };
	template<typename T>struct is_rval_ref :std::false_type { };
	template<typename T>struct is_rval_ref<T&&> :std::true_type { };
	template<typename T>struct is_ref :std::false_type { };
	template<typename T>struct is_ref<T&> :std::true_type { };
	template<typename T>struct is_ref<T&&> :std::true_type { };

	template<typename T> struct is_const :std::false_type { };
	template<typename T> struct is_const<const T> :std::true_type { };
	template<typename T> struct is_volatile :std::false_type { };
	template<typename T> struct is_volatile<volatile T> :std::true_type { };

	template<typename T1 , typename T2> using are_same_ignore_ref = are_same<rem_ref<T1> , rem_ref<T2>>;

	using llong = long long;
	using uint = unsigned int;
	using ulong = unsigned long;
	using ullong = unsigned long long;
	using ushort = unsigned short;
	using real = long double;

#pragma region Randomization
	std::mt19937_64 rand_gen;
	bool is_rand_gen_initialized = false;

	// Initializes the random number genreator with the current time as the seed.
	void rand_init()
	{
		if ( !is_rand_gen_initialized )
		{
			rand_gen =
				std::mt19937_64(
					std::chrono::steady_clock::now().
					time_since_epoch().count() );
			is_rand_gen_initialized = true;
		}
	}

	inline void rand_init( int seed )
	{
		rand_gen = std::mt19937_64( seed );
		is_rand_gen_initialized = true;
	}

	// Returns a random long integer number between min [inclusive] and max [exclusive]
	inline llong rand_long( llong min , llong max )
	{
		rand_init();
		return min + static_cast<utils::llong>( rand_gen() % ( max - min + 1 ) );
	}

	// Returns a random integer number between min [inclusive] and max [exclusive]
	inline int rand_int( int min , int max )
	{ return static_cast<int>( rand_long( min , max ) ); }

	// Returns a random element from the array you passed in
	template <typename Ds>
	inline auto rand_elem( const Ds& ds )-> decltype( ds[ 0 ] )
	{ return ds[ rand_int( 0 , N ) ]; }

	// Shuffles the array you passed in
	// WARNING: Will modify the array you passed in
	template <typename Ds>
	void shuffle_collection( const Ds& ds )
	{
		using std::swap;
		for ( int i = 0; i < N - 1; i++ )
			swap( ds[ rand_int( i , N ) ] , ds[ i ] );
	}
#pragma endregion

#pragma region Math Functions
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline uint abs( int value ) noexcept { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline float abs( float value ) noexcept { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline double abs( double value ) noexcept { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline real abs( real value ) noexcept { return ( value < 0 ? -value : value ); }

	// Repeats the value you passes in between 0 and max.
	uint repeat( int value , uint max )
	{
		if ( value < 0 )return repeat( max + uint( value ) , max );
		else if ( uint( value ) >= max )return value % max;
		else return value;
	}
	inline llong floor( real valueToFloor ) noexcept
	{ llong xi = llong( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	inline int floor( double valueToFloor ) noexcept
	{ int xi = int( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	inline int floor( float valueToFloor ) noexcept
	{ int xi = int( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	inline llong ceil( real valueToCeil ) noexcept
	{ llong xi = llong( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	inline int ceil( double valueToCeil ) noexcept
	{ int xi = int( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	inline int ceil( float valueToCeil ) noexcept
	{ int xi = int( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
#pragma endregion

	namespace operators
	{
	#pragma region Comparision
		struct less_than
		{
			constexpr inline less_than()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) < std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) < std::forward<T2>( v2 );
			}
		};
		struct less_than_or_eq
		{
			constexpr inline less_than_or_eq()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) <= std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) <= std::forward<T2>( v2 );
			}
		};
		struct greater_than
		{
			constexpr inline greater_than()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) > std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) > std::forward<T2>( v2 );
			}
		};
		struct greater_than_or_eq
		{
			constexpr inline greater_than_or_eq()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) >= std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) >= std::forward<T2>( v2 );
			}
		};
		struct equal
		{
			constexpr inline equal()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) == std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) == std::forward<T2>( v2 );
			}
		};
		struct not_equal
		{
			constexpr inline not_equal()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) != std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) != std::forward<T2>( v2 );
			}
		};
	#pragma endregion

	#pragma region Bitwise
		struct left_shift
		{
			constexpr inline left_shift()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) << std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) << std::forward<T2>( v2 );
			}
		};
		struct right_shift
		{
			constexpr inline right_shift()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) >> std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) >> std::forward<T2>( v2 );
			}
		};
		struct bitwise_and
		{
			constexpr inline bitwise_and()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) & std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) & std::forward<T2>( v2 );
			}
		};
		struct bitwise_xor
		{
			constexpr inline bitwise_xor()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) ^ std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) ^ std::forward<T2>( v2 );
			}
		};
		struct bitwise_or
		{
			constexpr inline bitwise_or()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) | std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) | std::forward<T2>( v2 );
			}
		};
	#pragma endregion

	#pragma region Logical
		struct logical_and
		{
			constexpr inline logical_and()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) && std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) && std::forward<T2>( v2 );
			}
		};
		struct logical_or
		{
			constexpr inline logical_or()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) || std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) || std::forward<T2>( v2 );
			}
		};
	#pragma endregion

	#pragma region Unary
		struct unary_minus
		{
			constexpr inline unary_minus()noexcept { }
			template<typename T>
			inline auto operator()( T&& v ) const ->
				decltype( -std::forward<T>( v ) )
			{
				return -std::forward<T>( v );
			}
		};
		struct negation
		{
			constexpr inline negation()noexcept { }
			template<typename T>
			inline auto operator()( T&& v ) const ->
				decltype( !std::forward<T>( v ) )
			{
				return !std::forward<T>( v );
			}
		};
		struct ones_complement
		{
			constexpr inline ones_complement()noexcept { }
			template<typename T>
			inline auto operator()( T&& v ) const ->
				decltype( ~std::forward<T>( v ) )
			{
				return ~std::forward<T>( v );
			}
		};
		struct address_of
		{
			constexpr inline address_of()noexcept { }
			template<typename T>inline T* operator()( T& v ) const { return &v; }
		};
		struct dereference
		{
			constexpr inline dereference()noexcept { }
			template<typename T>inline auto operator()( T&& v ) const ->
				decltype( *std::forward<T>( v ) )
			{
				return *std::forward<T>( v );
			}
		};
		struct increment
		{
			constexpr inline increment()noexcept { }
			template<typename T>inline auto operator()( T& v ) const ->
				decltype( ++v ) { return ++v; }
		};
		struct decrement
		{
			constexpr inline decrement()noexcept { }
			template<typename T>inline auto operator()( T& v ) const ->
				decltype( --v ) { return --v; }
		};
	#pragma endregion

	#pragma region Binary
		struct multiplication
		{
			constexpr inline multiplication()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) * std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) * std::forward<T2>( v2 );
			}
		};
		struct division
		{
			constexpr inline division()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) / std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) / std::forward<T2>( v2 );
			}
		};
		struct remainder
		{
			constexpr inline remainder()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) % std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) % std::forward<T2>( v2 );
			}
		};
		struct addition
		{
			constexpr inline addition()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) + std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) + std::forward<T2>( v2 );
			}
		};
		struct subtraction
		{
			constexpr inline subtraction()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 ) - std::forward<T2>( v2 ) )
			{
				return std::forward<T1>( v1 ) - std::forward<T2>( v2 );
			}
		};
		struct array_subscript
		{
			constexpr inline array_subscript()noexcept { }
			template<typename T1 , typename T2>
			inline auto operator()( T1&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T1>( v1 )[ std::forward<T2>( v2 ) ] )
			{
				return std::forward<T1>( v1 )[ std::forward<T2>( v2 ) ];
			}
		};
	#pragma endregion

		struct conditional
		{
			constexpr inline conditional() noexcept { }
			template<typename T>
			auto operator()( bool cond , T&& v1 , T&& v2 )->
				decltype( std::forward<T>( v1 ) )
			{
				return cond ? std::forward<T>( v1 ) : std::forward<T>( v2 );
			}
		};
	}

	// constants
	constexpr uint defaultLength = 5;
	constexpr uint defaultLengthIncrement = 5;
	constexpr int defaultEndIndex = -1;
}
#endif // !__UTILITIES__
