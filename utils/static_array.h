#pragma once
#include "utilities.h"
#include "operation_exception.h"
#include <type_traits>
#include <initializer_list>
namespace utils
{
	template<typename T , uint size>
	class static_array
	{
		T elementArray[ size ];
		constexpr const uint __Internal_CheckIndex( const uint index ) const
		{
			if ( llong( index ) > llong( size ) )
				throw operation_exception( from::static_array , error_type::indexTooLarge );
			return index;
		}
	public:
		enum { length = size };
		static_array( T fillVal = T() )
		{
			for ( uint i = 0; i < size; i++ )
				elementArray[ i ] = T();
		}
		static_array( static_array<T , size>&& arr ) noexcept
		{
			for ( uint i = 0; i < arr.length; i++ )
				elementArray[ i ] = std::move_if_noexcept( arr.elementArray[ i ] );
		}
		static_array( const static_array<T , size>& arr )
		{
			for ( uint i = 0; i < arr.length; i++ )
				elementArray[ i ] = arr.elementArray[ i ];
		}
		static_array( const std::initializer_list<T>& initList )
		{
			uint i = 0;
			for ( const T& v : initList )
			{
				elementArray[ i ] = v;
				i++;
			}
			for ( ; i < size; i++ )
				elementArray[ i ] = T();
		}
		static_array<T , size>& fill( const T& val = T() )
		{
			for ( uint i = 0; i < size; i++ )
				elementArray[ i ] = val;
			return *this;
		}
		static_array<T , size>& operator=(
			static_array<T , size>&& arr ) noexcept
		{
			for ( uint i = 0; i < arr.length; i++ )
				elementArray[ i ] =
				std::move_if_noexcept( arr.elementArray[ i ] );
			return *this;
		}
		static_array<T , size>& operator=(
			const static_array<T , size>& arr )
		{
			for ( uint i = 0; i < arr.length; i++ )
				elementArray[ i ] = arr.elementArray[ i ];
			return *this;
		}
		constexpr const T& operator[]( const int& index ) const
		{
			return elementArray[
				index < 0 ? ( length - 1 ) + index : index ];
		}
		T& operator[]( const int& index )
		{
			return elementArray[
				index < 0 ? ( length - 1 ) + index : index ];
		}
		constexpr const T& at( const int& index ) const
		{
			int Index = index;
			if ( Index < 0 )Index = endIndex + Index;
			return elementArray[ __Internal_CheckIndex( Index ) ];
		}
		T& at( const int& index )
		{
			int Index = index;
			if ( Index < 0 )Index = endIndex + Index;
			return elementArray[ __Internal_CheckIndex( Index ) ];
		}
		T* begin() const noexcept { return elementArray; }
		T* end() const noexcept { return elementArray + size; }
		std::reverse_iterator<T*> rbegin() const noexcept
		{ return ( std::reverse_iterator<T*>( elementArray + size ) ); }
		std::reverse_iterator<T*> rend() const noexcept
		{ return ( std::reverse_iterator<T*>( elementArray ) ); }
		constexpr bool empty() const noexcept
		{
			return false;
		}

		T& front() { return elementArray[ 0 ]; }
		constexpr const T& front() const { return elementArray[ 0 ]; }

		T& back() { return elementArray[ _Size - 1 ]; }
		constexpr const T& back() const
		{ return elementArray[ _Size - 1 ]; }

		T* data() { return elementArray; }
		constexpr const T* data() const { return elementArray; }
	};
}

