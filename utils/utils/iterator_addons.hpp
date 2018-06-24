#pragma once
#ifndef __UTILITIES__ITERATOR__ADDONS__
#define __UTILITIES__ITERATOR__ADDONS__
#include <type_traits>
#include <utility>
#include <iterator>
#include "utilities.hpp"
#include "static_array.hpp"
#include <tuple>
#include "type_at_index.hpp"
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

	template<typename iterator , typename iterator2>
	class iterator_pair
	{
	public:
		template<typename Iterator1 , typename Iterator2>
		iterator_pair( Iterator1&& _begin_ , Iterator2&& _end_ ) :
			_begin( std::forward<Iterator1>( _begin_ ) ) , 
			_end( std::forward<Iterator2>( _end_ ) ) { }

		iterator begin() { return _begin; }
		iterator2 end() { return _end; }
	private:
		iterator _begin;
		iterator2 _end;
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
	iterator_pair<rem_ref<Iterator1> , rem_ref<Iterator2>> pair_iterator(
		Iterator1&& _begin ,
		Iterator2&& _end )
	{
		return iterator_pair<rem_ref<Iterator1> , rem_ref<Iterator2>>
			(
				std::forward<Iterator1>( _begin ) ,
				std::forward<Iterator2>( _end )
				);
	}

#if __CPP_LANG > 201402
	// for_each_value_in_index_sequence_return_tuple_of_values
	template<typename TFunc , size_t... Indices>
	constexpr decltype( auto ) FEVIISRTOV(
		TFunc&& func ,
		std::index_sequence<Indices...> )
	{
		return std::forward_as_tuple(
			std::invoke(
			std::forward<TFunc>( func ) ,
			std::integral_constant<size_t , Indices>() )... );
	}

	template<typename TFunc , typename... TTuples>
	constexpr decltype( auto ) apply_to_each(
		TFunc&& func ,
		TTuples&&... tuples )
	{
		return FEVIISRTOV(
			[&] ( auto integral ) -> decltype( auto )
		{
			return std::forward<TFunc>( func )(
				std::get<integral.value>( std::forward<TTuples>( tuples ) )...
				);
		} ,
			std::make_index_sequence<
			std::tuple_size_v<std::decay_t<type_at_index<0 , TTuples...>>>>{} );
	}

	// for_each_value_in_index_sequence_return_value_returned_by_custom_function
	template<typename TFunc , typename TFuncConcat , size_t... Indices>
	constexpr decltype( auto ) FEVIISRVRBCF(
		TFunc&& func ,
		TFuncConcat&& func_concat ,
		std::index_sequence<Indices...> )
	{
		return std::forward<TFuncConcat>( func_concat )(
			std::invoke(
			std::forward<TFunc>( func ) ,
			std::integral_constant<size_t , Indices>() )... );
	}

	template<typename TFunc , typename TFuncConcat , typename... TTuples>
	constexpr decltype( auto ) apply_to_each_cus_concat(
		TFunc&& func ,
		TFuncConcat&& func_concat ,
		TTuples&&... tuples )
	{
		return FEVIISRVRBCF(
			[&] ( auto integral ) -> decltype( auto )
		{
			return std::forward<TFunc>( func )(
				std::get<integral.value>( std::forward<TTuples>( tuples ) )...
				);
		} ,
			std::forward<TFuncConcat>( func_concat ) ,
			std::make_index_sequence<
			std::tuple_size_v<std::decay_t<type_at_index<0 , TTuples...>>>>{} );
	}

	template<typename T , typename... Ts>
	class iter_zipper
	{
	public:
		using iter_curr_type = typename std::iterator_traits<T>::iterator_category;
		using iter_prev_type = typename iter_zipper<Ts...>::iterator_category;

		using iterator_category =
			select_t<
			std::is_base_of<iter_curr_type , iter_prev_type> , iter_curr_type ,
			select_t<std::is_base_of<iter_prev_type , iter_curr_type> , iter_prev_type , void>>;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type ,
			typename std::iterator_traits<Ts>::value_type...>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type& ,
			typename std::iterator_traits<Ts>::value_type&...>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type& ,
			const typename std::iterator_traits<Ts>::value_type&...>;
	private:
		std::tuple<T , Ts...> iters;
	public:

		iter_zipper( std::tuple<T , Ts...>&& tu ) :iters( std::move( tu ) )
		{ }

		iter_zipper( const std::tuple<T , Ts...>& tu ) :iters( tu ) { }

		iter_zipper<T , Ts...>& operator++()
		{
			apply_to_each_cus_concat(
				operators::increment() ,
				[] ( auto&&... ) { } ,
				iters );
			return *this;
		}

		iter_zipper<T , Ts...> operator++( int )
		{
			iter_zipper<T , Ts...> temp = *this;
			++( *this );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }

		reference ref()
		{ return apply_to_each( operators::dereference() , iters ); }

		inline bool operator!=( const iter_zipper<T , Ts...>& rhs )
		{
			return apply_to_each_cus_concat(
				operators::not_equal() ,
				[] ( auto&&... bools )
			{
				return ( std::forward<decltype( bools )>( bools ) && ... );
			} ,
				iters ,
				rhs.iters );
		}
		inline bool operator>( const iter_zipper<T , Ts...>& rhs )
		{ return iters > rhs.iters; }
		inline bool operator>=( const iter_zipper<T , Ts...>& rhs )
		{ return iters > rhs.iters; }
		inline bool operator<( const iter_zipper<T , Ts...>& rhs )
		{ return iters > rhs.iters; }
		inline bool operator<=( const iter_zipper<T , Ts...>& rhs )
		{ return iters > rhs.iters; }
	};
#endif

#pragma region iter_zipper
	template<typename T>
	class iter_zipper<T>
	{
	public:
		using iterator_category = typename std::iterator_traits<T>::iterator_category;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type&>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type&>;
	private:
		std::tuple<T> iters;
	public:

		template<typename T1_>
		iter_zipper( T1_&& v1 ) :
			iters( std::forward<T1_>( v1 ) )
		{ }

		iter_zipper<T>& operator++()
		{
			++std::get<0>( iters );
			++std::get<1>( iters );
			return *this;
		}

		iter_zipper<T> operator++( int )
		{
			iter_zipper<T> temp = *this;
			++std::get<0>( iters );
			++std::get<1>( iters );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }
		inline reference operator->() { return ref(); }
		inline const_reference operator->() const
		{ return ref(); }

		reference ref()
		{ return reference( *std::get<0>( iters ) ); }

		template<typename T1_ , typename T2_>
		bool operator!=( const iter_zipper<T1_ , T2_>& rhs )
		{
			return ( std::get<0>( iters ) != std::get<0>( rhs.iters ) );
		}
	};
	
	template<typename T , typename T2>
	class iter_zipper<T , T2>
	{
	public:
		using iter_curr_type = typename std::iterator_traits<T>::iterator_category;
		using iter_prev_type = typename std::iterator_traits<T2>::iterator_category;

		using iterator_category =
			select_t<
			std::is_base_of<iter_curr_type , iter_prev_type> , iter_curr_type ,
			select_t<std::is_base_of<iter_prev_type , iter_curr_type> , iter_prev_type , void>>;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type ,
			typename std::iterator_traits<T2>::value_type>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type& ,
			typename std::iterator_traits<T2>::value_type&>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type& ,
			const typename std::iterator_traits<T2>::value_type&>;
	private:
		std::tuple<T , T2> iters;
	public:

		iter_zipper( std::tuple<T , T2>&& tu ) :iters( std::move( tu ) )
		{ }

		iter_zipper( const std::tuple<T , T2>& tu ) :iters( tu ) { }

		iter_zipper<T , T2>& operator++()
		{
			++std::get<0>( iters );
			++std::get<1>( iters );
			return *this;
		}

		iter_zipper<T , T2> operator++( int )
		{
			iter_zipper<T , T2> temp = *this;
			++( *this );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }

		reference ref()
		{ return reference( *std::get<0>( iters ) , *std::get<1>( iters ) ); }

		inline bool operator!=( const iter_zipper<T , T2>& rhs )
		{
			return (
				std::get<0>( iters ) != std::get<0>( rhs.iters ) &&
				std::get<1>( iters ) != std::get<1>( rhs.iters )
				);
		}
	};

	template<typename T , typename T2 , typename T3>
	class iter_zipper<T , T2 , T3>
	{
	public:
		using iter_curr_type = typename std::iterator_traits<T>::iterator_category;
		using iter_prev_type = typename iter_zipper<T2 , T3>::iterator_category;

		using iterator_category =
			select_t<
			std::is_base_of<iter_curr_type , iter_prev_type> , iter_curr_type ,
			select_t<std::is_base_of<iter_prev_type , iter_curr_type> , iter_prev_type , void>>;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type ,
			typename std::iterator_traits<T2>::value_type ,
			typename std::iterator_traits<T3>::value_type>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type& ,
			typename std::iterator_traits<T2>::value_type& ,
			typename std::iterator_traits<T3>::value_type&>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type& ,
			const typename std::iterator_traits<T2>::value_type& ,
			const typename std::iterator_traits<T3>::value_type&>;
	private:
		std::tuple<T , T2 , T3> iters;
	public:

		iter_zipper( std::tuple<T , T2 , T3>&& tu ) :iters( std::move( tu ) )
		{ }

		iter_zipper( const std::tuple<T , T2 , T3>& tu ) :iters( tu ) { }

		iter_zipper<T , T2 , T3>& operator++()
		{
			++std::get<0>( iters );
			++std::get<1>( iters );
			++std::get<2>( iters );
			return *this;
		}

		iter_zipper<T , T2 , T3> operator++( int )
		{
			iter_zipper<T , T2 , T3> temp = *this;
			++( *this );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }

		reference ref()
		{
			return
				reference(
					*std::get<0>( iters ) ,
					*std::get<1>( iters ) ,
					*std::get<2>( iters ) );
		}

		inline bool operator!=( const iter_zipper<T , T2 , T3>& rhs )
		{
			return (
				std::get<0>( iters ) != std::get<0>( rhs.iters ) &&
				std::get<1>( iters ) != std::get<1>( rhs.iters ) &&
				std::get<2>( iters ) != std::get<2>( rhs.iters )
				);
		}
	};

	template<typename T , typename T2 , typename T3 , typename T4>
	class iter_zipper<T , T2 , T3 , T4>
	{
	public:
		using iter_curr_type = typename std::iterator_traits<T>::iterator_category;
		using iter_prev_type = typename iter_zipper<T2 , T3 , T4>::iterator_category;

		using iterator_category =
			select_t<
			std::is_base_of<iter_curr_type , iter_prev_type> , iter_curr_type ,
			select_t<std::is_base_of<iter_prev_type , iter_curr_type> , iter_prev_type , void>>;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type ,
			typename std::iterator_traits<T2>::value_type ,
			typename std::iterator_traits<T3>::value_type ,
			typename std::iterator_traits<T4>::value_type>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type& ,
			typename std::iterator_traits<T2>::value_type& ,
			typename std::iterator_traits<T3>::value_type& ,
			typename std::iterator_traits<T4>::value_type&>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type& ,
			const typename std::iterator_traits<T2>::value_type& ,
			const typename std::iterator_traits<T3>::value_type& ,
			const typename std::iterator_traits<T4>::value_type&>;
	private:
		std::tuple<T , T2 , T3 , T4> iters;
	public:

		iter_zipper( std::tuple<T , T2 , T3 , T4>&& tu ) :
			iters( std::move( tu ) )
		{ }

		iter_zipper( const std::tuple<T , T2 , T3 , T4>& tu ) :iters( tu ) { }

		iter_zipper<T , T2 , T3>& operator++()
		{
			++std::get<0>( iters );
			++std::get<1>( iters );
			++std::get<2>( iters );
			++std::get<3>( iters );
			return *this;
		}

		iter_zipper<T , T2 , T3 , T4> operator++( int )
		{
			iter_zipper<T , T2 , T3 , T4> temp = *this;
			++( *this );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }

		reference ref()
		{
			return
				reference(
					*std::get<0>( iters ) ,
					*std::get<1>( iters ) ,
					*std::get<2>( iters ) ,
					*std::get<3>( iters ) );
		}

		inline bool operator!=( const iter_zipper<T , T2 , T3 , T4>& rhs )
		{
			return (
				std::get<0>( iters ) != std::get<0>( rhs.iters ) &&
				std::get<1>( iters ) != std::get<1>( rhs.iters ) &&
				std::get<2>( iters ) != std::get<2>( rhs.iters ) &&
				std::get<3>( iters ) != std::get<3>( rhs.iters )
				);
		}
	};

	template<
		typename T ,
		typename T2 ,
		typename T3 ,
		typename T4 ,
		typename T5>
		class iter_zipper<T , T2 , T3 , T4 , T5>
	{
	public:
		using iter_curr_type = typename std::iterator_traits<T>::iterator_category;
		using iter_prev_type = typename iter_zipper<T2 , T3 , T4 , T5>::iterator_category;

		using iterator_category =
			select_t<
			std::is_base_of<iter_curr_type , iter_prev_type> , iter_curr_type ,
			select_t<std::is_base_of<iter_prev_type , iter_curr_type> , iter_prev_type , void>>;

		using value_type = std::tuple<
			typename std::iterator_traits<T>::value_type ,
			typename std::iterator_traits<T2>::value_type ,
			typename std::iterator_traits<T3>::value_type ,
			typename std::iterator_traits<T4>::value_type ,
			typename std::iterator_traits<T5>::value_type>;
		using reference = std::tuple<
			typename std::iterator_traits<T>::value_type& ,
			typename std::iterator_traits<T2>::value_type& ,
			typename std::iterator_traits<T3>::value_type& ,
			typename std::iterator_traits<T4>::value_type& ,
			typename std::iterator_traits<T5>::value_type&>;
		using const_reference = const std::tuple<
			const typename std::iterator_traits<T>::value_type& ,
			const typename std::iterator_traits<T2>::value_type& ,
			const typename std::iterator_traits<T3>::value_type& ,
			const typename std::iterator_traits<T4>::value_type& ,
			const typename std::iterator_traits<T5>::value_type&>;
	private:
		std::tuple<T , T2 , T3 , T4 , T5> iters;
	public:

		iter_zipper( std::tuple<T , T2 , T3 , T4 , T5>&& tu ) :
			iters( std::move( tu ) )
		{ }

		iter_zipper( const std::tuple<T , T2 , T3 , T4 , T5>& tu ) :iters( tu ) { }

		iter_zipper<T , T2 , T3>& operator++()
		{
			++std::get<0>( iters );
			++std::get<1>( iters );
			++std::get<2>( iters );
			++std::get<3>( iters );
			++std::get<4>( iters );
			return *this;
		}

		iter_zipper<T , T2 , T3 , T4 , T5> operator++( int )
		{
			iter_zipper<T , T2 , T3 , T4 , T5> temp = *this;
			++( *this );
			return std::move( temp );
		}

		inline reference operator*() { return ref(); }
		inline const_reference operator*() const
		{ return ref(); }
		inline reference operator->() { return ref(); }
		inline const_reference operator->() const
		{ return ref(); }

		reference ref()
		{
			return
				reference(
					*std::get<0>( iters ) ,
					*std::get<1>( iters ) ,
					*std::get<2>( iters ) ,
					*std::get<3>( iters ) ,
					*std::get<4>( iters ) );
		}

		inline bool operator!=( const iter_zipper<T , T2 , T3 , T4 , T5>& rhs )
		{
			return (
				std::get<0>( iters ) != std::get<0>( rhs.iters ) &&
				std::get<1>( iters ) != std::get<1>( rhs.iters ) &&
				std::get<2>( iters ) != std::get<2>( rhs.iters ) &&
				std::get<3>( iters ) != std::get<3>( rhs.iters ) &&
				std::get<4>( iters ) != std::get<4>( rhs.iters )
				);
		}
	};
#pragma endregion

	template<typename T , typename... Ts>
	iter_zipper<rem_ref<T> , rem_ref<Ts>...> zip( T&& t , Ts&&... ts )
	{
		return iter_zipper<rem_ref<T> , rem_ref<Ts>...>
			(
				std::make_tuple( std::forward<T>( t ) ,
				std::forward<Ts>( ts )... )
				);
	}

	template<typename T , typename... Ts>
	iterator_pair<
		iter_zipper<rem_ref<T> , rem_ref<Ts>...> ,
		iter_zipper<rem_ref<T> , rem_ref<Ts>...>>
		zip_colls( T&& t , Ts&&... ts )
	{
		return iterator_pair<
			iter_zipper<rem_ref<T> , rem_ref<Ts>...> ,
			iter_zipper<rem_ref<T> , rem_ref<Ts>...>>(
				iter_zipper<rem_ref<T> , rem_ref<Ts>...>
				(
				std::make_tuple( std::forward<T>( t ).begin() ,
				( std::forward<Ts>( ts ).begin() )... )
				) ,
				iter_zipper<rem_ref<T> , rem_ref<Ts>...>
				(
				std::make_tuple( std::forward<T>( t ).end() ,
				( std::forward<Ts>( ts ).end() )... )
				) );
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
	template<typename Char_t , typename Traits , typename Alloc , typename OutputIterator>
	void split( const std::basic_string<Char_t , Traits , Alloc>& text , OutputIterator output , const std::basic_string<Char_t>& delims )
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

	template<typename... Ts>
	struct iterator_traits<utils::iter_zipper<Ts...>>
	{
		using iterator_category = typename utils::iter_zipper<Ts...>::iterator_category;
		using value_type = typename utils::iter_zipper<Ts...>::value_type;
		using reference = typename utils::iter_zipper<Ts...>::reference;
	};
}
#endif // !__UTILITIES__ITERATOR__ADDONS__

