#pragma once
#ifndef __UTILITIES__OBJECT__
#define __UTILITIES__OBJECT__
#include <type_traits>
#include <memory>
#include "operation_exception.h"
#include "utilities.h"
namespace utils
{
	struct nullobj_t { };
	const nullobj_t nullobj;

	class object
	{
		template<typename T>
		using disable_cond = disable_if<
			or_t<
			are_same_ignore_ref<T , object> ,
			are_same_ignore_ref<T , nullobj_t>>>;
	public:
		object() :vref( nullptr ) { }
		template<typename T , disable_cond<T> = 0>
		object( T&& value ) : vref( new TPtr<typename std::decay<T>::type>( std::forward<T>( value ) ) ) { }
		object( const object& obj ) { obj.vref->CopyTo( vref ); }
		object( object&& obj ) :vref( std::move( obj.vref ) ) { }
		object( nullobj_t ) : vref( nullptr ) { }
		object& operator=( object&& obj )
		{
			vref = std::move( obj.vref );
			return *this;
		}
		object& operator=( const object& obj )
		{
			obj.vref->CopyTo( vref );
			return *this;
		}
		object& operator=( nullobj_t )
		{
			vref = nullptr;
			return *this;
		}
		template<typename T , disable_cond<T> = 0>
		object& operator=( T&& value )
		{
			vref.reset( new TPtr<typename std::decay<T>::type>( std::forward<T>( value ) ) );
			return *this;
		}
		template<typename T , typename... Args>
		void emplace( Args&&... args )
		{ vref.reset( new TPtr<T>( std::forward<Args>( args )... ) ); }
		template<typename T , typename... Args>
		void emplace_braced( Args&&... args )
		{ vref.reset( new TPtr<T>( T{ std::forward<Args>( args )... } ) ); }
		const std::type_info& type() const { return vref ? vref->GetTypeInfo() : typeid( void ); }
		inline explicit operator bool() const { return vref != nullptr; }

		inline bool operator==( nullobj_t ) const { return vref == nullptr; }
		inline bool operator!=( nullobj_t ) const { return vref != nullptr; }

		void swap( object& rhs ) { vref.swap( rhs.vref ); }

		~object() { }

	private:
		class ValidPtr
		{
		public:
			ValidPtr() { }
			virtual const std::type_info& GetTypeInfo() const = 0;
			virtual void CopyTo( std::unique_ptr<ValidPtr>& ptr ) const = 0;
			//virtual size_t GetHashCode() const = 0;
			virtual ~ValidPtr() { }
		};
		template<typename T> class TPtr : public ValidPtr
		{
		public:
			T value;
			template<typename... T_>
			TPtr( T_&&... value ) : value( std::forward<T_>( value )... ) { }
			// Inherited via ValidPtr
			virtual const std::type_info& GetTypeInfo() const override
			{ return typeid( value ); }
			/*virtual size_t GetHashCode() const override
			{ return std::hash<T>()( value ); }*/

			// Inherited via ValidPtr
			virtual void CopyTo( std::unique_ptr<ValidPtr>& ptr ) const override { ptr.reset( new TPtr<T>( value ) ); }
			virtual ~TPtr() override { }
		};
		std::unique_ptr<ValidPtr> vref;

		template<typename T>
		friend inline T obj_cast( object&& obj );
		template<typename T>
		friend inline T& obj_cast( const object& obj );
		template<typename T>
		friend inline T* obj_cast( const object* obj ) noexcept;
		//friend class std::hash<object>;
	};

	template<typename T>inline T* obj_cast( const object* obj ) noexcept
	{
		if ( !obj->vref )return nullptr;

		object::TPtr<T>* tptr = 
			dynamic_cast<object::TPtr<T>*>( obj->vref.get() );

		if ( !tptr )return nullptr;
		return &( tptr->value );
	}

	template<typename T>inline T& obj_cast( const object& obj )
	{
		T* ptr = obj_cast<T>( &obj );
		if ( !ptr )throw invalid_object_cast();
		return *ptr;
	}

	template<typename T>inline T obj_cast( object&& obj )
	{
		T* ptr = obj_cast<T>( &obj );
		if ( !ptr )throw invalid_object_cast();
		return std::move( *ptr );
	}
}
namespace std
{
	inline void swap( utils::object& lhs , utils::object& rhs ) noexcept { lhs.swap( rhs ); }

	//template<>
	//struct hash<utils::object>
	//{
	//	_NODISCARD size_t operator()( const utils::object& obj ) const
	//	{ return obj.vref->GetHashCode(); }
	//};

}
#endif // !__UTILITIES__OBJECT__

