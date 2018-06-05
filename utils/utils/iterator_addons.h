#pragma once
#ifndef __UTILITIES__ITERATOR__ADDONS__
#define __UTILITIES__ITERATOR__ADDONS__
#include <type_traits>
#include <utility>
#include <iterator>
#include "utilities.h"
#include "static_array.h"
namespace utils
{
#pragma region rec_iterator
	template<typename T , int N , typename TFunc>
	class rec_range;

	template<typename T , int ShiftValueSize , typename TFunc>
	class rec_iterator
	{
	public:
		using self = rec_iterator<T , ShiftValueSize , TFunc>;
		using value_type = T;
		using pointer = T * ;
		using reference = const T&;
		using const_reference = const T&;
		rec_iterator() = delete;

		inline pointer operator->() const
		{ return &regs.operator[]( ShiftValueSize - 1 ); }
		inline const_reference operator*() const noexcept
		{ return regs[ ShiftValueSize - 1 ]; }
		self& operator++()
		{
			static_array<T , ShiftValueSize> prev_regs = regs;
			for ( size_t i = 0; i < ShiftValueSize - 1; i++ )
				regs[ i ] = prev_regs[ i + 1 ];
			regs[ ShiftValueSize - 1 ] =
				func( prev_regs );
			++idex;
			return *this;
		}
		self operator+( T amount )
		{
			self temp = *this;
			for ( size_t i = 0; i < amount; i++ )
				temp++;
			return temp;
		}
		inline bool operator!=( const self &o )
			const noexcept { return idex != o.idex; }
		inline uint get_current_index() { return idex; }

	private:
		explicit inline rec_iterator(
			uint index ,
			const static_array<T , ShiftValueSize>& seed , const TFunc& func ) :
			idex{ index } , regs{ seed } , func{ func }
		{ }
		explicit inline rec_iterator(
			uint index ,
			const std::initializer_list<T>& seed , const TFunc& func ) :
			idex{ index } , regs{ seed } , func{ func }
		{ }

		uint idex{ 0 };
		static_array<T , ShiftValueSize> regs;
		const TFunc& func;

		template<typename T , int N , typename TFunc>
		friend class rec_range;
	};

	template<typename T , int N , typename TFunc>
	class rec_range
	{
	public:
		using iterator = rec_iterator<T , N , TFunc>;
		using value_type = T;
		using reference = const T&;
		using const_reference = const T&;
		using size_type = size_t;
		using const_iterator = rec_iterator<T , N , TFunc>;
	private:
		uint iterations;
		const static_array<T , N> seed;
		TFunc func;
	public:
		inline rec_range( const TFunc& func ,
						  uint iterations , const std::initializer_list<T>& seed ) :
			iterations{ iterations } , seed( seed ) , func{ func }
		{ }
		inline rec_range( const TFunc& func ,
						  uint iterations ,
						  const static_array<T , N>& seed ) :
			iterations{ iterations } , seed{ seed } , func{ func }
		{ }
		inline iterator begin() const noexcept
		{ return iterator{ 0,seed,func }; }
		inline iterator end() const
		{ return iterator{ iterations,{},func }; }
		constexpr size_t size() const noexcept
		{ return iterations; }
	};

	template<uint N , typename TFunc , typename T>
	rec_range<T , N , TFunc> make_range( const TFunc& func ,
										 uint iterations , const std::initializer_list<T>& seed )
	{
		return rec_range<T , N , TFunc>( func , iterations , seed );
	}
	template<uint N , typename TFunc , typename T >
	rec_range<T , N , TFunc> make_range( const TFunc& func ,
										 uint iterations , const static_array<T , N>& seed )
	{
		return rec_range<T , N , TFunc>( func , iterations , seed );
	}

	template<uint N , typename T>
	rec_range<T , N , T( *)( const static_array<T , N>& )> make_range(
		T( *func )( const static_array<T , N>& ) ,
		uint iterations , const std::initializer_list<T>& seed )
	{
		return rec_range<T , N , T( *)( const static_array<T , N>& )>( func , iterations , seed );
	}
	template<uint N , typename T >
	rec_range<T , N , T( *)( const static_array<T , N>& )> make_range(
		T( *func )( const static_array<T , N>& ) ,
		uint iterations , const static_array<T , N>& seed )
	{
		return rec_range<T , N , T( *)( const static_array<T , N>& )>( func , iterations , seed );
	}
#pragma endregion

	template<typename iterator>
	class iterator_pair
	{
	public:
		iterator begin() const { return _begin; }
		iterator end() const { return _end; }
	private:
		iterator _begin , _end;
	};

	template<typename T>
	class reverse_iterator_adapter
	{
		T& t;
	public:
		explicit reverse_iterator_adapter( T& t ) noexcept : t( t ) { }
		auto begin() noexcept( noexcept( t.rbegin() ) ) ->decltype( t.rbegin() ) { return t.rbegin(); }
		auto end() noexcept( noexcept( t.rend() ) ) ->decltype( t.rbegin() ) { return t.rend(); }
		auto begin() const noexcept( noexcept( t.rbegin() ) ) ->decltype( t.rbegin() ) { return t.rbegin(); }
		auto end() const noexcept( noexcept( t.rend() ) ) ->decltype( t.rbegin() ) { return t.rend(); }
	};

	template<typename T>
	reverse_iterator_adapter<T> reverse_adapt( T& t )
	{ return reverse_iterator_adapter<T>( t ); }

	template<typename Iterator1 , typename Iterator2>
	inline iterator_pair<rem_ref<Iterator1>> pair_iterator(
		Iterator1&& begin ,
		Iterator2&& end )
	{
		return iterator_pair<rem_ref<Iterator1>>
		{
			std::forward<Iterator1>( begin ) ,
				std::forward<Iterator2>( end )
		};
	}

#pragma region transform_if
	template<
		typename InputIterator ,
		typename OutputIterator ,
		typename ConditionFunction ,
		typename TransformationFunction>
		OutputIterator transform_if(
			InputIterator begin ,
			InputIterator end ,
			OutputIterator output ,
			ConditionFunction conditionFunction ,
			TransformationFunction transformationFunction )
	{
		for ( ; begin != end; ++begin )
			if ( conditionFunction( *begin ) )
			{
				*output = transformationFunction( *begin );
				output++;
			}
		return output;
	}
#pragma endregion
	template<typename Char_t , typename OutputIterator>
	void split( const std::basic_string<Char_t>& text , OutputIterator output , const std::basic_string<Char_t>& delims )
	{
		std::size_t start = text.find_first_not_of( delims ) ,
			end = 0;

		while (
			( end = text.find_first_of( delims , start ) ) != std::basic_string<Char_t>::npos )
		{
			*output = text.substr( start , end - start );
			output++;
			start = text.find_first_not_of( delims , end );
		}
		if ( start != std::basic_string<Char_t>::npos )
			*output = text.substr( start );
	}

	template<typename Char_t , typename OutputIterator>
	void split( const Char_t* text , OutputIterator&& output , const Char_t* delims )
	{
		split<Char_t>(
			std::basic_string<Char_t>( text ) ,
			std::forward<OutputIterator>( output ) ,
			std::basic_string<Char_t>( delims ) );
	}

#pragma region Iterator Trait Typedefs
	template<typename Iterator>
	using iter_diff =
		typename std::iterator_traits<Iterator>::difference_type;
	template<typename Iterator>
	using iter_val_t = typename std::iterator_traits<Iterator>::value_type;
	template<typename Iterator>
	using iter_category =
		typename std::iterator_traits<Iterator>::iterator_category;
#pragma endregion
}
namespace std
{
	template<typename T , int ShiftValueSize , typename TFunc>
	struct iterator_traits
		<utils::rec_iterator<T , ShiftValueSize , TFunc>>
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = typename remove_cv<T>::type;
		using pointer = T * ;
		using reference = T & ;
	};
}
#endif // !__UTILITIES__ITERATOR__ADDONS__

