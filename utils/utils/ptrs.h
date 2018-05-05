#pragma once
#ifndef __UTILITIES__GARBAGE_COLLECTION__POINTER_WRAPPERS__
#define __UTILITIES__GARBAGE_COLLECTION__POINTER_WRAPPERS__
#pragma push_macro("FLAG")
#pragma push_macro("FLAG_CT")
#pragma push_macro("FLAG_FUNCS_CT")
#undef FLAG
#undef FLAG_CT
#undef FLAG_FUNCS_CT
#include "macros\flags.h"
#include <type_traits>
#include <utility>
namespace utils
{
	FLAG( ptr_t )
	{
		unique = 1 << 0 ,
			abstract = 1 << 1 ,
	};

	template<typename T> struct ptr_traits
	{ static inline void dealloc( T* val ) noexcept { delete val; } };
	template<typename T> void ptr_traits<T>::dealloc( T* val ) noexcept;

	template<typename T , ptr_t TPtr = ptr_t::unique , typename TTraits = ptr_traits<T>>
	class ptr
	{
		T* valuePtr;
	public:
		static inline void dealloc( T* val ) noexcept { TTraits::dealloc( val ); }

		using traits = TTraits;
		using self = ptr<T , ptr_t::unique , TTraits>;
		template<typename _TTraits>
		using self_diff_trait = ptr<T , ptr_t::unique , _TTraits>;

		inline ptr( const self& ) = delete;
		inline ptr() noexcept :valuePtr{ nullptr } { }
		inline ptr( const std::nullptr_t ) noexcept : valuePtr{ nullptr } { }
		inline ptr( T* valuePtr ) noexcept : valuePtr{ valuePtr } { }
		inline ptr( self&& right ) noexcept : valuePtr{ right.valuePtr }
		{ right.valuePtr = nullptr; }

		inline self& operator=( const self& ) = delete;
		inline self& operator=( const std::nullptr_t )noexcept
		{
			dealloc( valuePtr );
			valuePtr = nullptr;
			return *this;
		}
		inline self& operator=( T* valuePtr )noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			this->valuePtr = valuePtr;
			return *this;
		}
		inline self& operator=( self&& right )noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = right.valuePtr;
			right.valuePtr = nullptr;
			return *this;
		}
		template<typename _TTraits>
		inline self& operator=( self_diff_trait<_TTraits>&& right )noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = right.valuePtr;
			right.valuePtr = nullptr;
			return *this;
		}

		inline explicit operator bool() const noexcept { return valuePtr != nullptr; }
		inline T& operator*() const noexcept { return *get(); }
		inline T* operator->() const noexcept { return get(); }

		inline T* release() noexcept
		{
			T* temp = valuePtr;
			valuePtr = nullptr;
			return temp;
		}
		inline void reset() noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = nullptr;
		}
		inline void reset( T* ptr ) noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = ptr;
		}
		template<typename... Args>
		inline void re_init( Args&&... args ) noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = new T();
			*valuePtr = T( std::forward<Args>( args )... );
		}
		template<typename... Args>
		inline void re_init_universal( Args&&... args ) noexcept
		{
			if ( valuePtr )dealloc( valuePtr );
			valuePtr = new T();
			*valuePtr = T{ std::forward<Args>( args )... };
		}
		~ptr() { if ( valuePtr )dealloc( valuePtr ); }

		template<typename _TTraits>
		inline void swap( self_diff_trait<_TTraits>& right ) noexcept
		{ std::swap( valuePtr , right.valuePtr ); }

		inline T* get() const noexcept { return valuePtr; }
	};

	template<typename T , typename TTraits>
	class ptr<T , ptr_t::abstract , TTraits>
	{
	protected:
		T * valuePtr;
		int* useCount;
	public:
		static inline void dealloc( T* val ) noexcept { TTraits::dealloc( val ); }
		using self = ptr<T , ptr_t::abstract , TTraits>;
		template<typename _TTraits>
		using self_diff_trait = ptr<T , ptr_t::abstract , _TTraits>;
		inline ptr() noexcept :valuePtr( nullptr ) , useCount( nullptr ) { }
		inline ptr( std::nullptr_t ) noexcept : valuePtr( nullptr ) , useCount( nullptr ) { }
		inline ptr( T* ptr ) noexcept : valuePtr( ptr ) , useCount( new int( 1 ) ) { }
		inline ptr( const self& sp ) noexcept : valuePtr( sp.valuePtr ) , useCount( sp.useCount ) { *useCount = *useCount + 1; }
		inline ptr( self&& sp ) noexcept : valuePtr( sp.valuePtr ) , useCount( sp.useCount ) { }
		self& operator=( const std::nullptr_t )
		{
			*useCount = *useCount - 1;
			if ( *useCount == 0 )
			{
				dealloc( valuePtr );
				delete useCount;
			}
			useCount = nullptr;
			valuePtr = nullptr;
			return *this;
		}
		template<typename T2>
		self& operator=( T2* ptr )
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					dealloc( valuePtr );
					delete useCount;
				}
			}
			useCount = new int( 1 );
			valuePtr = ptr;
			return *this;
		}
		self& operator=( T* ptr )
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					dealloc( valuePtr );
					delete useCount;
				}
			}
			useCount = new int( 1 );
			valuePtr = ptr;
			return *this;
		}
		self& operator=( const self& qp )
		{
			if ( useCount != nullptr )
			{
				dealloc( valuePtr );
				delete useCount;
			}
			valuePtr = qp.valuePtr;
			useCount = qp.useCount;
			*useCount = *useCount + 1;
			return *this;
		}
		self& operator=( self&& qp ) noexcept
		{
			if ( useCount != nullptr )
			{
				dealloc( valuePtr );
				delete useCount;
			}
			valuePtr = qp.valuePtr;
			useCount = qp.useCount;
			return *this;
		}
		// Returns true if the doesn't value exist
		bool operator==( std::nullptr_t ) { return valuePtr == nullptr; }
		// Returns true if the value exists
		bool operator!=( std::nullptr_t ) { return valuePtr != nullptr; }
		// Returns true if the value exists
		explicit operator bool() { return valuePtr != nullptr; }
		const T* operator->()const { return valuePtr; }
		const T& operator*() const { return *valuePtr; }
		T* operator->() { return valuePtr; }
		T& operator*() { return *valuePtr; }
		T* get() { return valuePtr; }
		template<typename... Args>
		void re_init_braced( Args&&... args )
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					if ( valuePtr != nullptr )
						dealloc( valuePtr );
					delete useCount;
				}
			}
			useCount = new int( 1 );
			valuePtr = new T{ std::forward<Args>( args )... };
		}
		template<typename... Args>
		void re_init( Args&&... args )
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					if ( valuePtr != nullptr )
						dealloc( valuePtr );
					delete useCount;
				}
			}
			useCount = new int( 1 );
			valuePtr = new T( std::forward<Args>( args )... );
		}
		T* release()
		{
			if ( useCount == nullptr )return nullptr;
			T* val = valuePtr;
			*useCount = *useCount - 1;
			if ( *useCount == 0 )
				delete useCount;
			useCount = new int( 1 );
			valuePtr = nullptr;
			return val;
		}
		inline void reset() noexcept
		{
			*useCount = *useCount - 1;
			if ( *useCount == 0 )
			{
				dealloc( valuePtr );
				delete useCount;
			}
			useCount = nullptr;
			valuePtr = nullptr;
		}
		inline void reset( T* ptr ) noexcept
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					dealloc( valuePtr );
					delete useCount;
				}
			}
			useCount = new int( 1 );
			valuePtr = ptr;
		}
		_declspec( property( get = get_use_count ) )int uses;
		_declspec( property( put = set_value , get = get_value ) )T value;
		const int get_use_count() { return *useCount; }

		template<typename _TTraits>
		inline void swap( self_diff_trait<_TTraits>& right ) noexcept
		{
			std::swap( valuePtr , right.valuePtr );
			std::swap( useCount , right.useCount );
		}

		~ptr()
		{
			if ( useCount == nullptr )return;
			*useCount = *useCount - 1;
			if ( *useCount == 0 )
			{
				dealloc( valuePtr );
				valuePtr = nullptr;
				delete useCount;
			}
		}
	};
	
	template<typename T , typename TTraits = ptr_traits<T>>
	using unique_pointer = ptr<T , ptr_t::unique , TTraits>;
	template<typename T , typename TTraits = ptr_traits<T>>
	using abstract_pointer = ptr<T , ptr_t::abstract , TTraits>;

	template<typename T , ptr_t TPtr = ptr_t::unique ,
		typename TTraits = ptr_traits<T> , typename... Args>
		inline ptr<T , TPtr , TTraits> make_ptr( Args&&... args )
	{
		return ptr<T , TPtr , TTraits>( T* p = new T( std::forward<Args>( args )... ) );
	}
	template<typename T , ptr_t TPtr = ptr_t::unique ,
		typename TTraits = ptr_traits<T> , typename... Args>
		inline ptr<T , TPtr , TTraits> make_ptr_universal( Args&&... args )
	{
		return ptr<T , TPtr , TTraits>( new T{ std::forward<Args>( args )... } );
	}

	template<typename T , ptr_t TPtr = ptr_t::unique ,
		typename TTraits = ptr_traits<T>>
		void ptr<T , TPtr , TTraits>::dealloc( T* val ) noexcept;
}

namespace std
{
	template<typename T , utils::ptr_t TPtr ,
		typename TTraits1 , typename TTraits2>
		inline void swap( utils::ptr<T , TPtr , TTraits1>& lhs , utils::ptr<T , TPtr , TTraits1>& rhs ) noexcept { return lhs.swap( rhs ); }
}

#undef FLAG
#undef FLAG_CT
#undef FLAG_FUNCS_CT
#pragma pop_macro("FLAG")
#pragma pop_macro("FLAG_CT")
#pragma pop_macro("FLAG_FUNCS_CT")  
#endif // !__UTILITIES__GARBAGE_COLLECTION__POINTER_WRAPPERS__
