#pragma once
#ifndef __UTILITIES__UTILITIES__
#define __UTILITIES__UTILITIES__
#include "macros\macros.hpp"
#include "type_at_index.hpp"
#include <tuple>
#include <random>
#include <chrono>
#include <type_traits>
#include <utility>
namespace utils
{
#pragma region type traits
	template<typename T , typename... Ts>struct and_t :
		std::bool_constant<T::value && and_t<Ts...>::value> { };
	template<typename T , typename... Ts>struct or_t :
		std::bool_constant<T::value || or_t<Ts...>::value> { };
	template<typename T , typename T2>struct and_t<T , T2> :
		std::bool_constant<T::value && T2::value> { };
	template<typename T , typename T2>struct or_t<T , T2> :
		std::bool_constant<T::value || T2::value> { };
	template<typename T>struct not_t :
		std::bool_constant<!T::value> { };

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

	template<bool , typename T , typename T2>struct _select_t
	{ using type = T; };
	template<typename T , typename T2>struct _select_t<false , T , T2>
	{ using type = T2; };
	template<typename Test , typename T , typename T2>
	using select_t = typename _select_t<Test::value , T , T2>::type;

	template<bool value>
	using val_to_t = std::bool_constant<value>;
#pragma endregion


	template<typename T , typename T2>struct are_same :std::false_type { };
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

	template<typename T , typename T2> using are_same_ignore_ref = are_same<rem_ref<T> , rem_ref<T2>>;

#pragma region does_exists
	template<typename...>using void_typ = void;
#if _HAS_CPP17
	namespace detail
	{
		template <
			typename Default ,
			typename AlwaysVoid ,
			template<class...> typename Op ,
			typename... Args>
			struct detector
		{
			using value_t = std::false_type;
			using type = Default;
		};

		template <
			typename Default ,
			template<class...> typename Op ,
			typename... Args>
			struct detector<Default , void_typ<Op<Args...>> , Op , Args...>
		{
			using value_t = std::true_type;
			using type = Op<Args...>;
		};

	}

	// special type to indicate detection failure
	struct null_ish
	{
		null_ish() = delete;
		~null_ish() = delete;
		null_ish( null_ish const& ) = delete;
		null_ish( null_ish&& ) = delete;
		null_ish& operator=( null_ish const& ) = delete;
		null_ish& operator=( null_ish&& ) = delete;
	};

	template <template<class...> class Op , class... Args>
	using does_exist =
		typename detail::detector<null_ish , void , Op , Args...>::value_t;

	template <template<class...> class Op , class... Args>
	using exists_t =
		typename detail::detector<null_ish , void , Op , Args...>::type;

	template <class Default , template<class...> class Op , class... Args>
	using exists_or = detail::detector<Default , void , Op , Args...>;
#endif // _HAS_CPP17

#pragma endregion


#pragma endregion

#pragma region using type-aliases
	using llong = long long;
	using uchar = unsigned char;
	using uint = unsigned int;
	using ulong = unsigned long;
	using ullong = unsigned long long;
	using ushort = unsigned short;
	using real = long double;
#pragma endregion

	template <typename T , size_t N>
	constexpr inline size_t size_of_coll( T( &)[ N ] )
	{ return N; }

	template <typename Ds>
	constexpr inline auto size_of_coll( const Ds& ds ) -> decltype( ds.size() )
	{ return ds.size(); }

#pragma region Randomization
	static std::mt19937_64 rand_gen;
	static bool is_rand_gen_initialized = false;

	// Initializes the random number genreator with the current time as the seed.
	static void rand_init()
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

	static inline void rand_init( int seed )
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
	inline auto rand_elem( Ds& ds )-> decltype( ds[ 0 ] )
	{ return ds[ rand_int( 0 , ds.size() ) ]; }

	template <typename T , size_t N>
	inline T& rand_elem( T( &a )[ N ] )
	{ return a[ rand_int( 0 , N ) ]; }

	// Shuffles the array you passed in
	// WARNING: Will modify the array you passed in
	template <typename Ds>
	void shuffle_collection( Ds& ds )
	{
		auto sz = ds.size();
		using std::swap;
		for ( auto i = typename Ds::size_type(); i < sz - 1; i++ )
			swap( ds[ rand_int( i , sz ) ] , ds[ i ] );
	}

	template <typename T , size_t N>
	void shuffle_collection( T( &a )[ N ] )
	{
		using std::swap;
		for ( size_t i = 0; i < N - 1; i++ )
			swap( ds[ rand_int( i , N ) ] , ds[ i ] );
	}
#pragma endregion

#pragma region Math Functions
#pragma region abs
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline ullong abs( llong value ) noexcept { return ullong( value < 0 ? -value : value ); }
	constexpr inline ulong abs( long value ) noexcept { return ulong( value < 0 ? -value : value ); }
	constexpr inline uint abs( int value ) noexcept { return uint( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline float abs( float value ) noexcept { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline double abs( double value ) noexcept { return ( value < 0 ? -value : value ); }
	// Returns the AbsoluteValue of the value you passed in.
	constexpr inline real abs( real value ) noexcept { return ( value < 0 ? -value : value ); }
#pragma endregion

#pragma region sign
	constexpr inline llong sign( llong value ) noexcept { return ( value < 0 ? -1 : 1 ); }
	constexpr inline long sign( long value ) noexcept { return ( value < 0 ? -1 : 1 ); }
	constexpr inline int sign( int value ) noexcept { return ( value < 0 ? -1 : 1 ); }
	constexpr inline float sign( float value ) noexcept { return ( value < 0 ? -1.f : 1.f ); }
	constexpr inline double sign( double value ) noexcept { return ( value < 0 ? -1. : 1. ); }
	constexpr inline real sign( real value ) noexcept { return ( value < 0 ? -1. : 1. ); }
#pragma endregion

#pragma region repeat
	// Repeats the value you passes in between min and max.
	constexpr inline llong repeat(
		llong value , ullong min , ullong max ) noexcept
	{
		return ( ( ( value + ( value < 0 ? -1 : 0 ) ) % ( max - min + 1 ) ) + min );
	}

	constexpr inline llong repeat( llong value , ullong max ) noexcept
	{ return ( ( value + ( value < 0 ? -1 : 0 ) ) % ( max + 1 ) ); }

	constexpr inline long repeat(
		long value , ulong min , ulong max ) noexcept
	{
		return ( ( ( value + ( value < 0 ? -1 : 0 ) ) % ( max - min + 1 ) ) + min );
	}

	constexpr inline long repeat( long value , ulong max ) noexcept
	{ return( ( value + ( value < 0 ? -1 : 0 ) ) % ( max + 1 ) ); }

	constexpr inline int repeat(
		int value , uint min , uint max ) noexcept
	{
		return ( ( ( value + ( value < 0 ? -1 : 0 ) ) % ( max - min + 1 ) ) + min );
	}

	constexpr inline int repeat( int value , uint max ) noexcept
	{ return ( ( value + ( value < 0 ? -1 : 0 ) ) % ( max + 1 ) ); }
#pragma endregion

#pragma region floor and ceil
	CPP_14_RELAXED_CONSTEXPR inline llong floor( real valueToFloor ) noexcept
	{ llong xi = llong( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	CPP_14_RELAXED_CONSTEXPR inline long floor( double valueToFloor ) noexcept
	{ int xi = long( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	CPP_14_RELAXED_CONSTEXPR inline int floor( float valueToFloor ) noexcept
	{ int xi = int( valueToFloor ); return valueToFloor < xi ? xi - 1 : xi; }
	CPP_14_RELAXED_CONSTEXPR inline llong ceil( real valueToCeil ) noexcept
	{ llong xi = llong( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	CPP_14_RELAXED_CONSTEXPR inline long ceil( double valueToCeil ) noexcept
	{ int xi = long( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
	CPP_14_RELAXED_CONSTEXPR inline int ceil( float valueToCeil ) noexcept
	{ int xi = int( valueToCeil ); return valueToCeil < xi ? xi : xi + 1; }
#pragma endregion
#pragma endregion

	namespace operators
	{
	#pragma region Comparision
		struct less_than
		{
			constexpr inline less_than()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) < std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) < std::forward<T2>( v2 );
			}
		};
		struct less_than_or_eq
		{
			constexpr inline less_than_or_eq()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) <= std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) <= std::forward<T2>( v2 );
			}
		};
		struct greater_than
		{
			constexpr inline greater_than()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) > std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) > std::forward<T2>( v2 );
			}
		};
		struct greater_than_or_eq
		{
			constexpr inline greater_than_or_eq()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) >= std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) >= std::forward<T2>( v2 );
			}
		};
		struct equal
		{
			constexpr inline equal()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) == std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) == std::forward<T2>( v2 );
			}
		};
		struct not_equal
		{
			constexpr inline not_equal()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) != std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) != std::forward<T2>( v2 );
			}
		};
	#pragma endregion

	#pragma region Bitwise
		struct left_shift
		{
			constexpr inline left_shift()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) << std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) << std::forward<T2>( v2 );
			}
		};
		struct right_shift
		{
			constexpr inline right_shift()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) >> std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) >> std::forward<T2>( v2 );
			}
		};
		struct bitwise_and
		{
			constexpr inline bitwise_and()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) & std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) & std::forward<T2>( v2 );
			}
		};
		struct bitwise_xor
		{
			constexpr inline bitwise_xor()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) ^ std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) ^ std::forward<T2>( v2 );
			}
		};
		struct bitwise_or
		{
			constexpr inline bitwise_or()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) | std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) | std::forward<T2>( v2 );
			}
		};
	#pragma endregion

	#pragma region Logical
		struct logical_and
		{
			constexpr inline logical_and()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) && std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) && std::forward<T2>( v2 );
			}
		};
		struct logical_or
		{
			constexpr inline logical_or()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) || std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) || std::forward<T2>( v2 );
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
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) * std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) * std::forward<T2>( v2 );
			}
		};
		struct division
		{
			constexpr inline division()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) / std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) / std::forward<T2>( v2 );
			}
		};
		struct remainder
		{
			constexpr inline remainder()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) % std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) % std::forward<T2>( v2 );
			}
		};
		struct addition
		{
			constexpr inline addition()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) + std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) + std::forward<T2>( v2 );
			}
		};
		struct subtraction
		{
			constexpr inline subtraction()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 ) - std::forward<T2>( v2 ) )
			{
				return std::forward<T>( v1 ) - std::forward<T2>( v2 );
			}
		};
		struct array_subscript
		{
			constexpr inline array_subscript()noexcept { }
			template<typename T , typename T2>
			inline auto operator()( T&& v1 , T2&& v2 ) const ->
				decltype( std::forward<T>( v1 )[ std::forward<T2>( v2 ) ] )
			{
				return std::forward<T>( v1 )[ std::forward<T2>( v2 ) ];
			}
		};
	#pragma endregion

		struct conditional
		{
			constexpr inline conditional() noexcept { }
			template<typename T , typename T2>
			auto operator()( bool cond , T&& v1 , T2&& v2 )->
				decltype( std::forward<T>( v1 ) )
			{
				return cond ? std::forward<T>( v1 ) : std::forward<T2>( v2 );
			}
		};

		using ternary_conditional = conditional;
	}

#if _HAS_CPP14
	template<typename Char , typename Traits , typename Alloc>
	auto trim( const std::basic_string<Char , Traits , Alloc>& s ) ->
		std::basic_string<Char , Traits , Alloc>
	{
		using std::begin;
		using std::end;
		using std::rbegin;
		using std::rend;
		using std::find_if_not;

		return std::basic_string<Char , Traits , Alloc>(
			find_if_not( begin( s ) , end( s ) , isspace ) ,
			find_if_not( rbegin( s ) , rend( s ) , isspace ).base()
			);
	}
#else
	template<typename Char , typename Traits , typename Alloc>
	auto trim( const std::basic_string<Char , Traits , Alloc>& s ) ->
		std::basic_string<Char , Traits , Alloc>
	{
		using std::begin;
		using std::end;
		using std::find_if_not;

		return std::basic_string<Char , Traits , Alloc>(
			find_if_not( begin( s ) , end( s ) , isspace ) ,
			find_if_not( s.rbegin() , s.rend() , isspace ).base()
			);
	}
#endif // _HAS_CPP14

	template<typename Char>
	auto trim( const Char* s , size_t len ) ->
		std::basic_string<Char>
	{
		using std::begin;
		using std::end;
		using std::find_if_not;

		return std::basic_string<Char>(
			find_if_not( s , s + len , isspace ) ,
			find_if_not( std::reverse_iterator<decltype( s )>( s ) , std::reverse_iterator<decltype( s )>( s + len ) , isspace ).base()
			);
	}

	template<typename Char>
	inline std::basic_string<Char> trim( const Char* s )
	{ return trim( s , strlen( s ) ); }

	struct null_t { };
	constexpr const null_t null;
	template<typename...>struct null_template { };

	template<typename T>
	inline std::string type_id()
	{
		static size_t null_template_len = strlen( typeid( null_template<> ).name() ) - 1;

		std::string str = typeid( null_template<T> ).name();
		return str.substr( null_template_len , str.length()- null_template_len  - 1 );
	}
}
#endif // !__UTILITIES__UTILITIES__
