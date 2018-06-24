#pragma once
#include "utilities.hpp"
#include <condition_variable>
#include <memory>
#include <functional>
#include <atomic>
#include <future>
#include <mutex>

#define UTILS_CORO_BREAK \
	yield.get_retainer()._Break();\
	return ::utils::null;
#define UTILS_CORO_YIELD(p) yield(p); if(yield.get_retainer()._Is_Cancelled())UTILS_CORO_BREAK;
namespace utils
{
	template<typename T>
	class retainer;
	template<typename T>
	class yielder;

	template<>
	class retainer<void>
	{
		using lock = std::unique_lock<std::mutex>;
		std::mutex mtx{ };
		std::condition_variable cvar_waiting_for_retain{};
		std::future<null_t> fun_fut;
		bool is_cancelled = false;
	public:
		std::condition_variable cvar_waiting_for_yield{ };
		template<typename TFunc , disable_if<std::is_base_of<retainer , rem_ref<TFunc>>> = 0>
		retainer( TFunc&& func ) :
			fun_fut( std::async(
				std::launch::async ,
				[this , &func] ()
		{
			return std::forward<TFunc>( func )(
				yielder<void>( *this ) );
		} ) )
		{
			fun_fut.wait_for( std::chrono::seconds( 0 ) );
		}
		retainer( retainer<void>&& ) = default;
		retainer<void>& operator=( retainer<void>&& ) = default;

		inline retainer& operator++()
		{
			retain();
			return *this;
		}

		inline bool operator!=( null_t ) const
		{
			return fun_fut.wait_for( std::chrono::seconds( 0 ) )
				!=std::future_status::ready;
		}

		inline null_t operator*() const noexcept { return null; }

		inline void _Break() noexcept { cvar_waiting_for_yield.notify_all(); }
		inline bool _Is_Cancelled() const noexcept { return is_cancelled; }

		~retainer()
		{
			is_cancelled = true;
			cvar_waiting_for_retain.notify_all();
			fun_fut.wait();
		}

	private:
		void yield()
		{
			cvar_waiting_for_yield.notify_all();
			{
				lock lk{ mtx };
				cvar_waiting_for_retain.wait( lk );
			}
		}
		void retain()
		{
			cvar_waiting_for_retain.notify_all();
			{
				lock lk{ mtx };
				cvar_waiting_for_yield.wait( lk );
			}
		}

		template<typename T>
		friend class yielder;
	};

	template<>
	class yielder<void>
	{
		retainer<void>&__retref__;
		yielder( retainer<void>& retref_ ) :__retref__( retref_ ) { }
		yielder& operator=( yielder&& ) = default;
		yielder( yielder&& ) = default;
	UTILS_DELETE_FUCTION_SCOPE:
		UTILS_DELETE_FUNCTION( yielder( const yielder& ) )
			UTILS_DELETE_FUNCTION( yielder& operator=( const yielder& ) )
	public:
		inline retainer<void>& get_retainer() noexcept { return __retref__; }
		inline const retainer<void>& get_retainer() const noexcept { return __retref__; }

		inline void operator()() { __retref__.yield(); }
		template<typename T>
		friend class retainer;
	};

	template<typename T>
	class retainer
	{
		using lock = std::unique_lock<std::mutex>;
		std::mutex mtx{ };
		std::condition_variable cvar_waiting_for_retain{};
		optional<T> value;
		std::future<null_t> fun_fut;
		bool is_cancelled = false;
	public:
		std::condition_variable cvar_waiting_for_yield{ };
		template<typename TFunc , disable_if<std::is_base_of<retainer , rem_ref<TFunc>>> = 0>
		retainer( TFunc&& func ) :
			fun_fut( std::async(
				std::launch::async ,
				[this , &func] ()
		{
			return std::forward<TFunc>( func )(
				yielder<T>( *this ) );
		} ) )
		{
			fun_fut.wait_for( std::chrono::seconds( 0 ) );
		}
		retainer( retainer<T>&& ) = default;
		retainer<T>& operator=( retainer<T>&& ) = default;

		inline retainer& operator++()
		{
			retain();
			return *this;
		}

		inline bool operator!=( null_t ) const
		{
			return fun_fut.wait_for( std::chrono::seconds( 0 ) )
				!=std::future_status::ready;
		}

		inline optional<T>& operator*() noexcept { return value; }
		inline const optional<T>& operator*() const noexcept { return value; }

		inline void _Break() noexcept { cvar_waiting_for_yield.notify_all(); }
		inline bool _Is_Cancelled() const noexcept { return is_cancelled; }

		~retainer()
		{
			is_cancelled = true;
			cvar_waiting_for_retain.notify_all();
			fun_fut.wait();
		}

	private:
		void yield( T&& val_init )
		{
			value = std::move( val_init );
			cvar_waiting_for_yield.notify_all();
			{
				lock lk{ mtx };
				cvar_waiting_for_retain.wait( lk );
			}
		}
		void yield( const T& val_init )
		{
			value = val_init;
			cvar_waiting_for_yield.notify_all();
			{
				lock lk{ mtx };
				cvar_waiting_for_retain.wait( lk );
			}
		}
		void retain()
		{
			cvar_waiting_for_retain.notify_all();
			{
				lock lk{ mtx };
				cvar_waiting_for_yield.wait( lk );
			}
		}

		template<typename T>
		friend class yielder;
	};

	template<typename T>
	class yielder
	{
		retainer<T>&__retref__;
		yielder( retainer<T>& retref_ ) :__retref__( retref_ ) { }
		yielder& operator=( yielder&& ) = default;
		yielder( yielder&& ) = default;
	UTILS_DELETE_FUCTION_SCOPE:
		UTILS_DELETE_FUNCTION( yielder( const yielder& ) )
			UTILS_DELETE_FUNCTION( yielder& operator=( const yielder& ) )
	public:
		inline retainer<T>& get_retainer() noexcept { return __retref__; }
		inline const retainer<T>& get_retainer() const noexcept { return __retref__; }

		inline void operator()( T&& value ) { __retref__.yield( std::move( value ) ); }
		inline void operator()( const T& value ) { __retref__.yield( value ); }
		template<typename T>
		friend class retainer;
	};
}

