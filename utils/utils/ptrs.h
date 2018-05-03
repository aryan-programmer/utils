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
namespace utils
{
	FLAG( ptr_t )
	{
		unique = 1 << 0 ,
			quick = 1 << 1 ,
			shared = 1 << 2 ,
			weak = 1 << 3 ,
			abstract = 1 << 4 ,
			intrusive = 1 << 5 ,
			safe = quick ,
			strong = shared
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

		inline T* get() const noexcept { return valuePtr; }
	};

	template<typename T , typename TTraits = ptr_traits<T>>
	using unique_pointer = ptr<T , ptr_t::unique , TTraits>;

	template<typename T , typename TTraits>
	class ptr<T , ptr_t::quick , TTraits>
	{
		T* vPtr;
		int* useCount;
	public:
		static inline void dealloc( T* val ) noexcept { TTraits::dealloc( val ); }

		using self = ptr<T , ptr_t::quick , TTraits>;
		ptr() noexcept :vPtr{ nullptr } , useCount{ nullptr } { }
		ptr( const std::nullptr_t ) noexcept : vPtr{ nullptr } , useCount{ nullptr } { }
		ptr( T* valuePtr ) noexcept :
			vPtr{ valuePtr } , useCount{ new int{ 1 } } { }
		ptr( const self& right ) noexcept :
			vPtr{ right.vPtr } , useCount{ right.useCount }
		{
			*useCount = ( ( *useCount ) + 1 );
		}

		self& operator=( const std::nullptr_t ) noexcept
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					delete vPtr;
					delete useCount;
				}
			}
			useCount = nullptr;
			vPtr = nullptr;
			return *this;
		}
		self& operator=( const self& right ) noexcept
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					delete vPtr;
					delete useCount;
				}
			}
			vPtr = right.vPtr;
			useCount = right.useCount;
			*useCount = *useCount + 1;
			return *this;
		}
		self& operator=( self&& right ) noexcept
		{
			if ( useCount != nullptr )
			{
				*useCount = *useCount - 1;
				if ( *useCount == 0 )
				{
					delete vPtr;
					delete useCount;
				}
			}
			vPtr = right.vPtr;
			useCount = right.useCount;
			right.vPtr = right.useCount = nullptr;
			return *this;
		}


		inline T* release() noexcept
		{
			if ( useCount == nullptr )return nullptr;
			T* val = vPtr;
			*useCount = *useCount - 1;
			if ( *useCount == 0 )
				delete useCount;
			useCount = nullptr;
			vPtr = nullptr;
			return val;
		}
		inline void reset() noexcept
		{
			T* p = release();
			if ( p != nullptr )
				delete p;
		}
		inline void reset( T* vptr ) noexcept
		{
			reset();
			useCount = new int{ 1 };
			vPtr = vptr;
		}
		template<typename... Args>
		inline void re_init( Args&&... args ) noexcept
		{ reset( new T( std::forward<Args>( args )... ) ); }
		template<typename... Args>
		inline void re_init_universal( Args&&... args ) noexcept
		{ reset( new T{ std::forward<Args>( args )... } ); }
		~ptr() { reset(); }
	};

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
#undef FLAG
#undef FLAG_CT
#undef FLAG_FUNCS_CT
#pragma pop_macro("FLAG")
#pragma pop_macro("FLAG_CT")
#pragma pop_macro("FLAG_FUNCS_CT")  
#endif // !__UTILITIES__GARBAGE_COLLECTION__POINTER_WRAPPERS__
