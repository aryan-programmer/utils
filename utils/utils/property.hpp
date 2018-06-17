#pragma once

#ifndef __UTILITIES__PROPERTY__
#define __UTILITIES__PROPERTY__
#include "utilities.hpp"

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

		using rvalue_reference = t&&;
		using rref = t&&;

		using reference = t&;
		using const_reference = const t&;
		using ref = t&;
		using cref = const t&;

		using pointer = t*;
		using const_pointer = const t*;
		using ptr = t*;
		using cptr = const t*;

	public:
		// Getters
		virtual inline operator cref() const & = 0;
		virtual inline cref operator*() const = 0;
		virtual inline cptr operator->()const = 0;

		// Modifiers / modifiable reference getters
		virtual inline ref get() = 0;
		virtual inline operator ref () & = 0;
		virtual inline operator rref ()&&= 0;
		virtual inline ref operator*() = 0;
		virtual inline ptr operator->() = 0;

		virtual inline ref set( rref v ) = 0;
		virtual inline ref set( cref v ) = 0;

		virtual inline ref operator=( rref v ) = 0;
		virtual inline ref operator=( cref v ) = 0;
	};

	template<typename T, typename Parent = utils::null_t>
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
		t raw;

	public:
		template<typename... Args ,
			disable_if<is_property_ignore_ref<type_at_index<0 , Args...>>> = 0>
			property( Args... args ) : raw{ std::forward<Args>( args )... } { }
		property( const self& v ) : raw{ v.raw } { }
		property( self&& v ) : raw{ std::move( v.raw ) } { }

		// Getters
		inline operator cref() const & override { return raw; }
		inline cref operator*() const override { return raw; }
		inline cptr operator->()const override { return &raw; }

		// Modifiers / modifiable reference getters
		inline ref get() override { return raw; }
		inline operator ref () & override { return raw; }
		inline operator rref ()&&override {return std::move( raw ); }
		inline ref operator*() override { return raw; }
		inline ptr operator->() override { return &raw; }

		template<typename T_>
		inline ref set( T_&& v ) { return raw = std::forward<T_>( v ); }

		inline ref set( rref v ) { return raw = v; }
		inline ref set( cref v ) { return raw = v; }

		inline ref operator=( rref v ) { return raw = v; }
		inline ref operator=( cref v ) { return raw = v; }

		template<typename T_ , disable_if<are_same_ignore_ref<T_ , self>> = 0>
		inline ref operator=( T_&& v )
		{ return raw = std::forward<T_>( v ); }
		inline self& operator=( const self& v )
		{
			raw = v.raw;
			return *this;
		}
		inline self& operator=( self&& v )
		{
			raw = std::move( v.raw );
			return *this;
		}

		friend typename Parent;
	};
}
#endif // !__UTILITIES__PROPERTY__
