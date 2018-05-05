#pragma once
#include <string>
#include <type_traits>
#include "utils\ptrs.h"
#include "utils\operation_exception.h"
namespace utils
{
	class object
	{
	public:
		object() :vref( nullptr ) { }
		template<typename T>
		object( T&& value ) : vref( new TPtr<T>( std::forward<T>( value ) ) ) { }
		object( const object& obj ) :vref( obj.vref ) { }
		object( object&& obj ) :vref( std::move( obj.vref ) ) { }
		object& operator=( object&& obj )
		{
			vref = std::move( obj.vref );
			return *this;
		}
		object& operator=( const object& obj )
		{
			vref = obj.vref;
			return *this;
		}
		template<typename T>
		object& operator=( const T& value )
		{
			vref = new TPtr<T>( value );
			return *this;
		}
		template<typename T>
		T& get()
		{
			TPtr<T>* tptr = dynamic_cast<TPtr<T>*>( vref.get() );

			if ( !tptr )
				throw operation_exception( from::object , error_type::invalidObjectCast );
			else if ( !has_value() )
				throw operation_exception( from::object , error_type::valueNotPresent );
			return *tptr->GetRef();
		}
		template<typename T>
		const T& get() const
		{
			TPtr<T>* tptr = dynamic_cast<TPtr<T>*>( vref.get() );

			if ( !tptr )
				throw operation_exception( from::object , error_type::invalidObjectCast );
			else if ( !has_value() )
				throw operation_exception( from::object , error_type::valueNotPresent );
			return *tptr->GetRef();
		}
		template<typename T>
		T* get_ptr()
		{
			TPtr<T>* tptr = dynamic_cast<TPtr<T>*>( vref.get() );

			if ( !tptr || !has_value() )
				return nullptr;
			return tptr->GetRef();
		}
		template<typename T>
		const T* get_ptr() const
		{
			TPtr<T>* tptr = dynamic_cast<TPtr<T>*>( vref.get() );

			if ( !tptr || !has_value() )
				return nullptr;
			return tptr->GetRef();
		}
		const std::string get_type() { return vref->GetTypeInfo(); }
		const bool has_value() { return vref != nullptr; }

		inline void swap( object& rhs ) noexcept { vref.swap( rhs.vref ); }

		~object() { }

	private:
		class ValidPtr
		{
		public:
			ValidPtr() { }
			virtual std::string GetTypeInfo() = 0;
			virtual ~ValidPtr() { }
		};
		template<typename T> class TPtr : public ValidPtr
		{
		public:
			TPtr( const T& value ) :value( value ) { }
			typename std::remove_reference<T>::type* GetRef() { return &value; }
			std::string GetTypeInfo() override { return typeid( value ).name(); }
			~TPtr() override { }
			T value;
		};
		abstract_pointer<ValidPtr> vref;
	};
}
namespace std
{
	inline void swap( utils::object& lhs , utils::object& rhs ) noexcept { lhs.swap( rhs ); }
}
