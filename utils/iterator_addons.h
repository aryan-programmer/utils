#pragma once
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
			const static_array<T , ShiftValueSize>& seed , TFunc func ) :
			idex{ index } , regs{ seed } , func{ func }
		{ }
		explicit inline rec_iterator(
			uint index ,
			const std::initializer_list<T>& seed , TFunc func ) :
			idex{ index } , regs{ seed } , func{ func }
		{ }

		uint idex{ 0 };
		static_array<T , ShiftValueSize> regs;
		TFunc func;

		template<typename T , int N , typename TFunc>
		friend class rec_range;
	};

	template<typename T , int N , typename TFunc>
	class rec_range
	{
	public:
		using iterator = rec_iterator<T , N , TFunc>;
	private:
		uint iterations;
		const static_array<T , N> seed;
		TFunc func;
	public:
		inline rec_range( TFunc func ,
						  uint iterations , const std::initializer_list<T>& seed ) :
			iterations{ iterations } , seed{ seed } , func{ func }
		{ }
		inline rec_range( TFunc func ,
						  uint iterations ,
						  const static_array<T , N> seed ) :
			iterations{ iterations } , seed{ std::move( seed ) } , func{ func }
		{ }
		inline iterator begin() const
		{ return iterator{ 0,seed,func }; }
		inline iterator end() const
		{ return iterator{ iterations,{},func }; }
	};

	template<typename iterator>
	class iterator_pair
	{
	public:
		explicit inline iterator_pair( iterator begin , iterator end ) noexcept :
			_begin{ begin } , _end{ end } { }

		iterator& begin() const { return _begin; }
		iterator& end() const { return _end; }
	private:
		iterator _begin , _end;
	};

	template<typename T>
	class reverse_iterator_adapter
	{
		T& t;
	public:
		explicit reverse_iterator_adapter( T& t ) noexcept : t( t ) { }
		auto begin() ->decltype( t.rbegin() ) { return t.rbegin(); }
		auto end()->decltype( t.rbegin() ) { return t.rend(); }
		auto begin() const->decltype( t.rbegin() ) { return t.rbegin(); }
		auto end() const->decltype( t.rbegin() ) { return t.rend(); }
	};

	template<int N , typename TFunc , typename T >
	rec_range<T , N , TFunc> make_range( TFunc func ,
										 uint iterations , const std::initializer_list<T>& seed )
	{
		return rec_range<T , N , TFunc>( func , iterations , seed );
	}
	template<int N , typename TFunc , typename T >
	rec_range<T , N , TFunc> make_range( TFunc func ,
										 uint iterations , const static_array<T , N>& seed )
	{
		return rec_range<T , N , TFunc>( func , iterations , seed );
	}
#pragma endregion

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

}
namespace std
{
	template<typename T , int ShiftValueSize , typename TFunc>
	struct iterator_traits
		<utils::rec_iterator<T , ShiftValueSize , TFunc>>
	{
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using pointer = T * ;
		using reference = T & ;
	};
}
