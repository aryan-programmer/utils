#pragma once
#pragma push_macro("FLAG")
#pragma push_macro("FLAG_CT")
#pragma push_macro("FLAG_FUNCS_CT")
#undef FLAG
#undef FLAG_CT
#undef FLAG_FUNCS_CT
#include "flag_macros.h"
#include <type_traits>
namespace utils
{
	FLAG( ptr_t )
	{
		unique = 1 << 0 ,
			shared = 1 << 1 ,
			strong = 1 << 2 ,
			weak = 1 << 3 ,
			abstract = 1 << 4
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
		inline self& operator=( const T* valuePtr )noexcept
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

	template<typename T , ptr_t TPtr = ptr_t::unique , typename TTraits = ptr_traits<T> , typename... Args>
	inline ptr<T , TPtr , TTraits> make_ptr( Args&&... args )
	{
		return ptr<T , TPtr , TTraits>( T* p = new T( std::forward<Args>( args )... ) );
	}
	template<typename T , ptr_t TPtr = ptr_t::unique , typename TTraits = ptr_traits<T> , typename... Args>
	inline ptr<T , TPtr , TTraits> make_ptr_universal( Args&&... args )
	{
		return ptr<T , TPtr , TTraits>( new T{ std::forward<Args>( args )... } );
	}

	template<typename T , ptr_t TPtr = ptr_t::unique , typename TTraits = ptr_traits<T>>
	void ptr<T , TPtr , TTraits>::dealloc( T* val ) noexcept;

	template<typename T , typename TTraits = ptr_traits<T>>
	using unique_pointer = ptr<T , ptr_t::unique , TTraits>;
}
#undef FLAG
#undef FLAG_CT
#undef FLAG_FUNCS_CT
#pragma pop_macro("FLAG")
#pragma pop_macro("FLAG_CT")
#pragma pop_macro("FLAG_FUNCS_CT")
