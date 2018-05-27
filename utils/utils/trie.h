#pragma once
#ifndef __UTILITIES__TRIE__
#define __UTILITIES__TRIE__
#include <map>
#include <string>
#include <list>
#include "utilities.h"
#include "optional.h"
#include "iterator_addons.h"

namespace utils
{
	template<typename T>
	struct pair_t_void;
	template<typename T>
	struct pair_void_t;

	template<typename T1 , typename T2>
	using pair =
		select_t<
		are_same_ignore_ref<T1 , void> ,
		pair_void_t<T1> ,
		select_t<
		are_same_ignore_ref<T2 , void> ,
		pair_t_void<T1> ,
		std::pair<T1 , T2>>>;


	template<typename T>
	struct pair_t_void
	{
		using first_type = T;
		using second_type = void;

		constexpr pair_t_void() : first{} { }

		template<typename ignore>
		constexpr pair_t_void( const T& val , const ignore& ) : first( val ) { }

		pair_t_void( const pair_t_void& ) = default;
		pair_t_void( pair_t_void&& ) = default;

		template<typename OT1>
		pair_t_void( const pair_t_void<OT1>& rhs ) : first( rhs.first ) { }
		template<typename OT1>
		pair_t_void( pair_t_void<OT1>&& rhs ) :
			first( std::move( rhs.first ) ) { }

		template<typename OT1>
		pair_t_void( const pair_void_t<OT1>& ) : first{} { }
		template<typename OT1>
		pair_t_void( pair_void_t<OT1>&& ) : first{} { }

		template<typename OT1 , typename OT2>
		constexpr pair_t_void( const std::pair<OT1 , OT2>& rhs ) : first( rhs.first ) { }

		template<typename OT1 , typename OT2>
		pair_t_void& operator=( const std::pair<OT1 , OT2>& rhs )
		{
			first = rhs.first;
			return *this;
		}

		template<typename OT , typename ignore>
		constexpr explicit pair_t_void( OT&& v , ignore&& )
			noexcept( std::is_nothrow_constructible<T , OT>::value )
			: first( std::forward<OT>( v ) )
		{ }

		template<typename OT , typename ignore>
		pair_t_void& operator=( std::pair<OT , ignore>&& rhs )
		{
			first = std::move( rhs.first );
			return *this;
		}

		pair_t_void& operator=( pair_t_void&& rhs )
			noexcept( std::is_nothrow_move_assignable<T>::value )
		{
			first = std::move( rhs.first );
			return *this;
		}

		pair_t_void& operator=( const pair_t_void& rhs )
		{
			first = rhs.first;
			return *this;
		}

		template<typename OT>
		pair_t_void& operator=( pair_t_void<OT>&& rhs )
			noexcept( std::_Is_nothrow_convertible<OT , T>::value )
		{
			first = std::move( rhs.first );
			return *this;
		}

		template<typename OT>
		pair_t_void& operator=( const pair_t_void<OT>& rhs )
		{
			first = rhs.first;
			return *this;
		}

		template<typename OT>
		pair_t_void& operator=( pair_void_t<OT>&& )noexcept { return *this; }

		template<typename OT>
		pair_t_void& operator=( const pair_void_t<OT>& rhs ) noexcept
		{ return *this; }

		void swap( pair_t_void& rhs )
			noexcept( std::_Is_nothrow_swappable<T>::value )
		{
			using std::swap;
			swap( rhs.first , first );
		}

		T first;
	};

	template<typename T>
	struct pair_void_t
	{
		using second_type = T;
		using first_type = void;

		constexpr pair_void_t() : second{} { }

		template<typename ignore>
		constexpr pair_void_t( const T& val , const ignore& ) : second( val ) { }

		pair_void_t( const pair_void_t& ) = default;
		pair_void_t( pair_void_t&& ) = default;

		template<typename OT1>
		pair_void_t( const pair_void_t<OT1>& ) : second( rhs.second ) { }
		template<typename OT1>
		pair_void_t( pair_void_t<OT1>&& ) :
			second( std::move( rhs.second ) ) { }

		template<typename OT1>
		pair_void_t( const pair_t_void<OT1>& ) : second{} { }
		template<typename OT1>
		pair_void_t( pair_t_void<OT1>&& ) : second{} { }

		template<typename OT1 , typename OT2>
		constexpr pair_void_t( const std::pair<OT1 , OT2>& rhs ) : second( rhs.second ) { }

		template<typename OT1 , typename OT2>
		pair_void_t& operator=( const std::pair<OT1 , OT2>& rhs )
		{
			second = rhs.second;
			return *this;
		}

		template<typename OT , typename ignore>
		constexpr explicit pair_void_t( OT&& v , ignore&& )
			noexcept( std::is_nothrow_constructible<T , OT>::value )
			: second( std::forward<OT>( v ) )
		{ }

		template<typename OT , typename ignore>
		pair_void_t& operator=( std::pair<OT , ignore>&& rhs )
		{
			second = std::move( rhs.second );
			return *this;
		}

		pair_void_t& operator=( pair_void_t&& rhs )
			noexcept( std::is_nothrow_move_assignable<T>::value )
		{
			second = std::move( rhs.second );
			return *this;
		}

		pair_void_t& operator=( const pair_void_t& rhs )
		{
			second = rhs.second;
			return *this;
		}

		template<typename OT>
		pair_void_t& operator=( pair_void_t<OT>&& rhs )
			noexcept( std::_Is_nothrow_convertible<OT , T>::value )
		{
			second = std::move( rhs.second );
			return *this;
		}

		template<typename OT>
		pair_void_t& operator=( const pair_void_t<OT>& rhs )
		{
			second = rhs.second;
			return *this;
		}

		template<typename OT>
		pair_void_t& operator=( pair_t_void<OT>&& )noexcept { return *this; }

		template<typename OT>
		pair_void_t& operator=( const pair_t_void<OT>& rhs ) noexcept
		{ return *this; }

		void swap( pair_void_t& rhs )
			noexcept( std::_Is_nothrow_swappable<T>::value )
		{
			using std::swap;
			swap( rhs.second , second );
		}

		T second;
	};

	template<typename T , typename CommentType>
	struct trie_node
	{
		using self = trie_node<T , CommentType>;
		std::map<T , self> table;
		using is_word_and_comment_t =
			select_t<
			are_same_ignore_ref<CommentType , void> ,
			pair<bool , void> ,
			pair<bool , optional<CommentType>>>;
		using const_is_word_and_dynamic_comment_ref_t =
			select_t<
			are_same_ignore_ref<CommentType , void> ,
			pair<const bool& , void> ,
			pair<const bool& , optional<CommentType>&>>;
		is_word_and_comment_t is_word_and_comment;
		trie_node() :table{ } , is_word_and_comment{ } { }
		trie_node( std::nullptr_t ) :table{ } , is_word_and_comment{ } { }
		//void remove_word( const T& c )
		//{
		//	auto it = table.find( c );
		//	if ( it != table.end() )
		//		table.erase( it );
		//}
		trie_node( self&& rhs ) :
			table( std::move( rhs.table ) ) ,
			is_word_and_comment( std::move( rhs.is_word_and_comment ) )
		{
			rhs.is_word_and_comment = is_word_and_comment_t{};
		}
		self& operator=( self&& rhs )
		{
			table = std::move( rhs.table );
			is_word_and_comment = std::move( rhs.is_word_and_comment );
			rhs.is_word_and_comment = is_word_and_comment_t{};
			return *this;
		}
		trie_node( const self& rhs ) :table( rhs.table ) ,
			is_word_and_comment( rhs.is_word_and_comment )
		{ }
		self& operator=( const self& rhs )
		{
			table = rhs.table;
			is_word_and_comment = rhs.is_word_and_comment;
			return *this;
		}
		/*inline self& follow_value( const T& value )
		{ return table.find( c )->second; }*/
		const_is_word_and_dynamic_comment_ref_t get_CIWADyCRef()
		{
			return
				const_is_word_and_dynamic_comment_ref_t
				( is_word_and_comment );
		}
	};

	template<typename T = char , typename ValueType = std::basic_string<T> , typename CommentType = void>
	class trie
	{
		using trie_node_t = trie_node<T , CommentType>;
		trie_node_t root;
	public:
		using indexing_value = typename trie_node_t::const_is_word_and_dynamic_comment_ref_t;
		using orig_map_value = typename trie_node_t::is_word_and_comment_t;
		using value_type = ValueType;
		using key_type = ValueType;
		using mapped_type = optional<indexing_value>;
		trie() :root{} { }
		mapped_type insert( const ValueType& ds_v )
		{
			trie_node_t* n = &root;
			for ( auto& value : ds_v )
				n = &n->table.emplace( value , nullptr ).first->second;
			n->is_word_and_comment.first = true;
			return n->get_CIWADyCRef();
		}
		mapped_type insert( ValueType&& ds_v )
		{
			trie_node_t* n = &root;
			for ( auto& value : ds_v )
				n = &n->
				table.
				emplace( std::move( value ) , nullptr ).
				first->second;
			n->is_word_and_comment.first = true;
			return n->get_CIWADyCRef();
		}
		inline bool contains( const ValueType& ds_v )
		{ return at( ds_v ).has_value(); }
		inline bool is_prefix( const ValueType& ds_v )
		{ return at_prefix( ds_v ).has_value(); }
		template<typename Ds>
		inline void push_back( Ds&& ds_v )
		{ insert( std::forward<Ds>( ds_v ) ); }
		template<typename Ds , typename... Dss>
		void insert_all( Ds&& ds_v , Dss&&... ds_vs )
		{
			insert( std::forward<Ds>( ds_v ) );
			insert_all( std::forward<Dss>( ds_vs )... );
		}
		template<typename Ds>
		inline void insert_all( Ds&& ds_v )
		{ insert( std::forward<Ds>( ds_v ) ); }
		template<typename Dss>
		inline void insert_array_of_values( Dss&& dss )
		{
			if ( is_rval_ref<Dss>::value )
				for ( auto& ds : std::forward<Dss>( dss ) )
					insert( ds );
			else
				for ( auto& ds : std::forward<Dss>( dss ) )
					insert( std::move( ds ) );
		}

		optional<orig_map_value> remove_word( const ValueType& ds_v )
		{
			trie_node_t* n = &root;
			std::list<trie_node_t*> path{};
			optional<orig_map_value> ret_val = nullopt;
			for ( auto& value : ds_v )
			{
				auto iter_to_value = n->table.find( value );
				if ( iter_to_value == n->table.end() ) return nullopt;
				path.push_back( n );
				n = &iter_to_value->second;
			}
			path.push_back( n );
			ret_val = n->is_word_and_comment;
			if ( n->is_word_and_comment.first == false )return nullopt;
			n->is_word_and_comment.first = false;

			/// Delete un-needed nodes
			auto input_iter = ds_v.end();
			--input_iter;
			auto it = path.end();
			--it;
			--it;
			for ( ; it != path.begin(); it-- )
			{
				if ( n->table.empty() &&
					( n->is_word_and_comment.first == false ) )
				{
					n = *it;
					auto erase_it = n->table.find( *input_iter );
					if ( erase_it != n->table.end() )
						n->table.erase( erase_it );
					--input_iter;
				}
				else break;
			}
			return ret_val;
		}

		mapped_type at_prefix( const ValueType& ds_v )
		{
			trie_node_t* n = &root;
			for ( auto& value : ds_v )
			{
				auto iter_to_value = n->table.find( value );
				if ( iter_to_value == n->table.end() ) return nullopt;
				n = &iter_to_value->second;
			}
			return n->get_CIWADyCRef();
		}

		mapped_type at( const ValueType& ds_v )
		{
			auto ret_val = at_prefix( ds_v );
			if ( !ret_val )return nullopt;
			if ( ret_val->first == false )return nullopt;
			return ret_val;
		}

		inline mapped_type operator[]( const ValueType& ds_v )
		{
			auto ret_val = at( ds_v );
			if ( !ret_val )return insert( ds_v );
			return ret_val;
		}

		inline mapped_type operator[]( ValueType&& ds_v )
		{
			auto ret_val = at( ds_v );
			if ( !ret_val )return insert( std::forward<ValueType>( ds_v ) );
			return ret_val;
		}
	};
}
#endif // !__UTILITIES__TRIE__
