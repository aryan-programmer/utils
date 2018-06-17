#pragma once
#ifndef __UTILITIES__OBJECT__
#define __UTILITIES__OBJECT__
#include <type_traits>
#include <memory>
#include "operation_exception.hpp"
#include "utilities.hpp"
namespace utils
{
	class object
	{
	protected:
		template<typename T>
		using disable_cond = disable_if<
			or_t<
			std::is_base_of<rem_ref<T> , object> ,
			std::is_base_of<rem_ref<T> , null_t>>>;
	public:
		object() :vref( nullptr ) { }
		template<typename T , disable_cond<T> = 0>
		object( T&& value ) : vref( new TPtr<typename std::decay<T>::type>( std::forward<T>( value ) ) ) { }
		object( const object& obj ) { obj.GetVref()->CopyTo( GetVref() ); }
		object( object&& obj ) :vref( std::move( obj.GetVref() ) ) { }
		object( null_t ) : vref( nullptr ) { }
		object& operator=( object&& obj )
		{
			GetVref() = std::move( obj.GetVref() );
			return *this;
		}
		object& operator=( const object& obj )
		{
			obj.GetVref()->CopyTo( GetVref() );
			return *this;
		}
		object& operator=( null_t )
		{
			GetVref() = nullptr;
			return *this;
		}
		template<typename T , disable_cond<T> = 0>
		object& operator=( T&& value )
		{
			GetVref().reset( new TPtr<typename std::decay<T>::type>( std::forward<T>( value ) ) );
			return *this;
		}
		template<typename T , typename... Args>
		void emplace( Args&&... args )
		{ GetVref().reset( new TPtr<T>( std::forward<Args>( args )... ) ); }
		template<typename T , typename... Args>
		void emplace_braced( Args&&... args )
		{ GetVref().reset( new TPtr<T>( T{ std::forward<Args>( args )... } ) ); }
		const std::type_info& type() const { return GetVref() ? GetVref()->GetTypeInfo() : typeid( void ); }
		inline explicit operator bool() const { return GetVref() != nullptr; }

		inline bool operator==( null_t ) const { return GetVref() == nullptr; }
		inline bool operator!=( null_t ) const { return GetVref() != nullptr; }

		void swap( object& rhs ) { GetVref().swap( rhs.GetVref() ); }

		~object() { }

	protected:
		class ValidPtr
		{
		public:
			ValidPtr() { }
			virtual void* get() const = 0;
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

			// Inherited via ValidPtr
			virtual void * get() const override { return &value; }
		};
		virtual inline std::unique_ptr<ValidPtr>& GetVref() noexcept
		{ return vref; }
		virtual inline const std::unique_ptr<ValidPtr>& GetVref() const noexcept
		{ return vref; }
		std::unique_ptr<ValidPtr> vref;

		template<typename T>
		friend inline typename std::decay<T>::type* obj_cast_unsafe( const object* obj ) noexcept;

		template<typename T>
		friend typename std::decay<T>::type* obj_cast( const object* obj ) noexcept;
		//friend class std::hash<object>;
	};

	class object_reference : public object
	{
	protected:
		template<typename T>
		using disable_cond = disable_if<
			or_t<
			std::is_base_of<rem_ref<T> , object> ,
			std::is_base_of<rem_ref<T> , null_t>>>;
	public:
		object_reference() :vref( nullptr ) { }
		template<typename T , disable_cond<T> = 0>
		object_reference( T& ref ) : vref( new TPtr<T>( ref ) ) { }
		object_reference( const object_reference& obj ) { obj.GetVref()->CopyTo( GetVref() ); }
		object_reference( object_reference&& obj ) :vref( std::move( obj.GetVref() ) ) { }
		object_reference( null_t ) : vref( nullptr ) { }
		object_reference& operator=( object_reference&& obj )
		{
			GetVref() = std::move( obj.GetVref() );
			return *this;
		}
		object_reference& operator=( const object_reference& obj )
		{
			obj.GetVref()->CopyTo( GetVref() );
			return *this;
		}
		object_reference& operator=( null_t )
		{
			GetVref() = nullptr;
			return *this;
		}
		template<typename T , disable_cond<T> = 0>
		object_reference& operator=( T& ref )
		{
			GetVref().reset( new TPtr<T>( ref ) );
			return *this;
		}

		template<typename T>typename std::decay<T>::type* as_ptr() const noexcept
		{
			if ( !GetVref() )return nullptr;

			object_reference::TPtr<T>* tptr =
				dynamic_cast<object_reference::TPtr<T>*>( GetVref().get() );

			if ( !tptr )return nullptr;
			return &( tptr->value );
		}

		template<typename T>typename T& as_ref() const & noexcept
		{
			auto ptr = as_ptr<T>();
			if ( !ptr )throw invalid_object_cast();
			return *ptr;
		}
		template<typename T>typename T as_ref() && noexcept
		{
			auto ptr = as_ptr<T>();
			if ( !ptr )throw invalid_object_cast();
			return std::move( *ptr );
		}

		template<typename T>
		inline typename std::decay<T>::type* as_ptr_unsafe() const noexcept
		{
			if ( !GetVref() )return nullptr;
			return static_cast<typename std::decay<T>::type*>( GetVref()->get() );
		}

		template<typename T>inline typename T& as_ref_unsafe() const & noexcept
		{ return *as_ptr_unsafe<T>(); }
		template<typename T>inline typename T as_ref_unsafe() && noexcept
		{return std::move( *as_ptr_unsafe<T>() ); }

		~object_reference() { }

	protected:
		template<typename T> class TPtr : public ValidPtr
		{
		public:
			T & ref;
			TPtr( T& ref_ ) : ref( ref_ ) { }
			// Inherited via ValidPtr
			virtual const std::type_info& GetTypeInfo() const override
			{ return typeid( ref ); }

			// Inherited via ValidPtr
			virtual void CopyTo( std::unique_ptr<ValidPtr>& ptr ) const override { ptr.reset( new TPtr<T>( ref ) ); }
			virtual ~TPtr() override { }

			// Inherited via ValidPtr
			virtual void * get() const override { return &ref; }
		};
		virtual inline std::unique_ptr<ValidPtr>& GetVref() noexcept
		{ return vref; }
		virtual inline const std::unique_ptr<ValidPtr>& GetVref() const noexcept
		{ return vref; }
		std::unique_ptr<ValidPtr> vref;
	};

	template<typename T>
	inline typename std::decay<T>::type* obj_cast_unsafe(
		const object_reference* obj ) noexcept
	{
		return obj->as_ptr_unsafe<T>();
	}

	template<typename T>inline T& obj_cast_unsafe( const object_reference& obj ) noexcept
	{ return obj.as_ref_unsafe<T>(); }

	template<typename T>inline T obj_cast_unsafe( object_reference&& obj ) noexcept
	{ return std::move( obj.as_ref_unsafe<T>() ); }

	template<typename T>
	inline typename std::decay<T>::type* obj_cast(
		const object_reference* obj ) noexcept
	{
		return obj->as_ptr<T>();
	}

	template<typename T>inline T& obj_cast( const object_reference& obj )
	{ return obj.as_ref<T>(); }

	template<typename T>inline T obj_cast( object_reference&& obj )
	{ return obj.as_ref<T>(); }

	template<typename T>typename std::decay<T>::type* obj_cast( const object* obj ) noexcept
	{
		if ( !obj->GetVref() )return nullptr;

		object::TPtr<T>* tptr =
			dynamic_cast<object::TPtr<T>*>( obj->GetVref().get() );

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

	template<typename T>
	inline typename std::decay<T>::type* obj_cast_unsafe( const object* obj ) noexcept
	{
		if ( !obj->GetVref() )return nullptr;
		return static_cast<typename std::decay<T>::type*>( obj->GetVref()->get() );
	}

	template<typename T>inline T& obj_cast_unsafe( const object& obj ) noexcept
	{ return *obj_cast_unsafe<T>( &obj ); }

	template<typename T>inline T obj_cast_unsafe( object&& obj ) noexcept
	{ return std::move( *obj_cast_unsafe<T>( &obj ) ); }
}
namespace std
{
	inline void swap( utils::object& lhs , utils::object& rhs ) noexcept { lhs.swap( rhs ); }

	//template<>
	//struct hash<utils::object>
	//{
	//	_NODISCARD size_t operator()( const utils::object& obj ) const
	//	{ return obj.GetVref()->GetHashCode(); }
	//};

}
#endif // !__UTILITIES__OBJECT__

