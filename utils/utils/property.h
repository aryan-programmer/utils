#pragma once

#ifndef __UTILITIES__PROPERTY__
#define __UTILITIES__PROPERTY__
#include "utilities.h"

namespace utils
{
	struct property_tag { };

	template<typename T>
	using is_property_ignore_ref = std::is_base_of<rem_ref<property_tag> , rem_ref<T>>;

	template<typename T>
	class i_property :property_tag
	{
	public:
		using t = rem_ref<T>;

		using rvalue_reference = t && ;
		using rref = t && ;

		using reference = t & ;
		using const_reference = const t&;
		using ref = t & ;
		using cref = const t&;

		using pointer = t * ;
		using const_pointer = const t*;
		using ptr = t * ;
		using cptr = const t*;

	public:
		// Getters
		virtual inline operator cref() const & = 0;
		virtual inline cref operator*() const = 0;
		virtual inline cptr operator->()const = 0;

		// Modifiers / modifiable reference getters
		virtual inline ref get() = 0;
		virtual inline operator ref () & = 0;
		virtual inline operator rref () && = 0;
		virtual inline ref operator*() = 0;
		virtual inline ptr operator->() = 0;

		virtual inline ref set( rref v ) = 0;
		virtual inline ref set( cref v ) = 0;

		virtual inline ref operator=( rref v ) = 0;
		virtual inline ref operator=( cref v ) = 0;
	};

	template<typename T>
	class property : public i_property<T>
	{
	public:
		using base = i_property<T>;

		using t = typename base::t;

		using rvalue_reference = typename base::rvalue_reference;
		using rref = typename base::rref;

		using reference = typename base::reference;
		using const_reference = typename base::const_reference;
		using ref = typename base::ref;
		using cref = typename base::cref;

		using pointer = typename base::pointer;
		using const_pointer = typename base::const_pointer;
		using ptr = typename base::ptr;
		using cptr = typename base::cptr;

		using self = property<T>;

	private:
		ref _ref;

	public:
		property( ref r ) : _ref{ r } { }
		property( const self& v ) : _ref{ v._ref } { }
		property( self&& v ) : _ref{ std::move( v._ref ) } { }

		// Getters
		inline operator cref() const & override { return _ref; }
		inline cref operator*() const override { return _ref; }
		inline cptr operator->()const override { return &_ref; }

		// Modifiers / modifiable reference getters
		inline ref get() override { return _ref; }
		inline operator ref () & override { return _ref; }
		inline operator rref () && override {return std::move( _ref ); }
		inline ref operator*() override { return _ref; }
		inline ptr operator->() override { return &_ref; }

		template<typename T_>
		inline ref set( T_&& v ) { return _ref = std::forward<T_>( v ); }

		inline ref set( rref v ) { return _ref = v; }
		inline ref set( cref v ) { return _ref = v; }

		inline ref operator=( rref v ) { return _ref = v; }
		inline ref operator=( cref v ) { return _ref = v; }

		template<typename T_ , disable_if<are_same_ignore_ref<T_ , self>> = 0>
		inline ref operator=( T_&& v )
		{ return _ref = std::forward<T_>( v ); }
		inline self& operator=( const self& v )
		{
			_ref = v._ref;
			return *this;
		}
		inline self& operator=( self&& v )
		{
			_ref = std::move( v._ref );
			return *this;
		}
	};
}
#endif // !__UTILITIES__PROPERTY__
