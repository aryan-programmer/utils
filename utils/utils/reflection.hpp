#pragma once
#include "object.hpp"
#include "utilities.hpp"
#include "iterator_addons.hpp"
#include <array>
#include <unordered_map>
#include <boost\preprocessor\seq\for_each_i.hpp>
#include <boost\preprocessor\variadic\to_seq.hpp>
#include <boost\preprocessor\punctuation\comma_if.hpp>
#include <boost\type_index.hpp>

#define CUSTOM_REFLECT ::utils::reflection::reflector reflect_all()

namespace utils
{
	namespace reflection
	{
		template<typename T>
		struct reflector_wrapper;
		struct reflector;
		struct reflectable { };
		template<size_t size>
		class reflector_zipper;
		class reflected_variable_ref;
		struct deny_reflection { };
		struct mirror_t
		{
			mirror_t() { }
			mirror_t( mirror_t&& ) = delete;
			mirror_t( const mirror_t& ) = delete;
			mirror_t& operator=( const mirror_t& ) = delete;
			mirror_t& operator=( mirror_t&& ) = delete;

			template<typename T>
			inline reflector operator<( reflector_wrapper<T>&& rflwp )
			{ return rflwp.ref.reflect_all(); }
		};

		mirror_t mirror;

	#if _HAS_CPP17
		template<typename T>
		using _is_reflectable = decltype( &T::reflect_all );
		template<typename T>
		using _is_denying_reflection = decltype( &T::operator~ );
		template<typename T>
		using is_reflectable = and_t<
			does_exist<_is_reflectable , T> ,
			not_t<or_t<
			does_exist<_is_denying_reflection , T> ,
			std::is_base_of<deny_reflection , T>>>>;
	#elif
		template<typename T>
		using is_reflectable = and_t<std::is_base_of<reflectable , T> , not_t<std::is_base_of<deny_reflection , T>>>;
	#endif // _HAS_CPP17

		struct reflector
		{
			using ref_map_t = std::unordered_map<std::string , reflected_variable_ref>;
			template<typename Iter1 , typename Iter2>
			reflector( Iter1&& v1 , Iter2&& v2 , ref_map_t::size_type buckets ) :
				refs( std::forward<Iter1>( v1 ) , std::forward<Iter2>( v2 ) , buckets ) { }
			reflector() :refs{ } { }
			reflected_variable_ref& operator[]( const std::string& str )
			{ return refs.at( str ); }
			reflected_variable_ref& operator[]( std::string&& str )
			{ return refs.at( str ); }
		private:
			ref_map_t refs;
		};

		class reflected_variable_ref : public reflectable
		{
		private:
			template<typename T>
			using disable_cond = disable_if<
				or_t<
				std::is_base_of<rem_ref<T> , reflected_variable_ref> ,
				std::is_base_of<rem_ref<T> , null_t>>>;
		public:
			reflected_variable_ref() :vref( nullptr ) { }
			template<typename T , disable_cond<T> = 0>
			reflected_variable_ref( T& value ) :
				vref( new TPtr<typename std::decay<T>::type>( value ) ) { }
			reflected_variable_ref( const reflected_variable_ref& obj )
			{ obj.vref->CopyTo( vref ); }
			reflected_variable_ref( reflected_variable_ref&& obj )
			{ obj.vref->CopyTo( vref ); }
			reflected_variable_ref& operator=( reflected_variable_ref&& obj )
			{
				vref->RetrieveFrom( std::move( obj.vref ) );
				return *this;
			}
			reflected_variable_ref& operator=( const reflected_variable_ref& obj )
			{
				vref->RetrieveFrom( obj.vref );
				return *this;
			}
			const std::type_info& type() const { return vref ? vref->GetTypeInfo() : typeid( void ); }
			inline explicit operator bool() const { return vref!=nullptr; }

			inline bool operator==( null_t ) const { return vref==nullptr; }
			inline bool operator!=( null_t ) const { return vref!=nullptr; }

			inline void swap( reflected_variable_ref& rhs ) { vref->Swap( rhs.vref ); }

			template<typename T>typename std::decay<T>::type* as_ptr() const noexcept
			{
				if(!vref)return nullptr;

				TPtr<T>* tptr = dynamic_cast<TPtr<T>*>( vref.get() );

				if(!tptr)return nullptr;
				return &( tptr->value );
			}

			template<typename T>typename T& as_ref() const & noexcept
			{
				auto ptr = as_ptr<T>();
				if(!ptr)throw invalid_object_cast();
				return *ptr;
			}
			template<typename T>typename T as_ref()&&noexcept
			{
				auto ptr = as_ptr<T>();
				if(!ptr)throw invalid_object_cast();
				return std::move( *ptr );
			}

			template<typename T>
			inline typename std::decay<T>::type* as_ptr_unsafe() const noexcept
			{
				if(!vref)return nullptr;
				return static_cast<typename std::decay<T>::type*>( vref->get() );
			}

			template<typename T>inline typename T& as_ref_unsafe() const & noexcept
			{ return *as_ptr_unsafe<T>(); }
			template<typename T>inline typename T as_ref_unsafe()&&noexcept
			{return std::move( *as_ptr_unsafe<T>() ); }

			CUSTOM_REFLECT
			{
				return vref->reflect_all();
			};

			~reflected_variable_ref() { }

		private:
			class ValidPtr
			{
			public:
				virtual inline const std::type_info& GetTypeInfo() const = 0;
				virtual void CopyTo( std::unique_ptr<ValidPtr>& ptr ) = 0;
				virtual void RetrieveFrom( const std::unique_ptr<ValidPtr>& ptr ) = 0;
				virtual void RetrieveFrom( std::unique_ptr<ValidPtr>&& ptr ) = 0;
				virtual void Swap( std::unique_ptr<ValidPtr>& ptr ) = 0;
				virtual inline reflector reflect_all() = 0;
				virtual void* get() = 0;
				virtual ~ValidPtr() { }
			};
			template<typename T> class TPtr : public ValidPtr
			{
			public:
				T&ref;
				TPtr( T& ref_ ) : ref( ref_ ) { }
				// Inherited via ValidPtr
				virtual inline const std::type_info& GetTypeInfo() const override
				{ return typeid( ref ); }

				// Inherited via ValidPtr
				virtual void RetrieveFrom( const std::unique_ptr<ValidPtr>& ptr ) override
				{
					TPtr<T>* ptr_t = dynamic_cast<TPtr<T>*>( ptr.get() );
					if(!ptr_t)throw bad_dynamic_assignment();
					ref = ptr_t->ref;
				}
				// Inherited via ValidPtr
				virtual void RetrieveFrom( std::unique_ptr<ValidPtr>&& ptr ) override
				{
					TPtr<T>* ptr_t = dynamic_cast<TPtr<T>*>( ptr.get() );
					if(!ptr_t)throw bad_dynamic_assignment();
					ref = std::move( ptr_t->ref );
				}

				virtual void Swap( std::unique_ptr<ValidPtr>& ptr ) override
				{
					TPtr<T>* ptr_t = dynamic_cast<TPtr<T>*>( ptr.get() );
					if(!ptr_t)throw bad_dynamic_assignment();
					std::swap( ref , ptr_t->ref );
				}

				virtual void CopyTo( std::unique_ptr<ValidPtr>& ptr )
				{
					ptr.reset( new TPtr<T>( ref ) );
				}

				virtual void* get()override { return &ref; }

				virtual ~TPtr() override { }

				// Inherited via ValidPtr
				virtual inline reflector reflect_all() override
				{
					return reflect_all( is_reflectable<rem_ref<T>>() );
				}
				inline reflector reflect_all( std::true_type ) { return ref.reflect_all(); }
				inline reflector reflect_all( std::false_type ) { return reflector{}; }
			};
			std::unique_ptr<ValidPtr> vref;
		};
		template<size_t size>
		class reflector_zipper
		{
		public:
			using iterator_category = std::random_access_iterator_tag;

			using value_type = std::pair<std::string , reflected_variable_ref>;
			using reference = std::pair<std::string& , reflected_variable_ref&&>;
		private:
			using iter_pair = std::pair<typename std::array<std::string , size>::iterator ,
				std::move_iterator<typename std::array<reflected_variable_ref , size>::iterator>>;
			iter_pair iters;
		public:

			reflector_zipper( iter_pair&& tu ) :iters( std::move( tu ) )
			{ }

			reflector_zipper( const iter_pair& tu ) :iters( tu ) { }

			reflector_zipper<size>& operator++()
			{
				++iters.first;
				++iters.second;
				return *this;
			}

			reflector_zipper<size> operator++( int )
			{
				reflector_zipper<size> temp = *this;
				++( *this );
				return std::move( temp );
			}

			inline reference operator*() { return ref(); }

			reference ref()
			{
				return reference( *iters.first , *iters.second );
			}

			inline bool operator!=( const reflector_zipper<size>& rhs )
			{
				return iters!=rhs.iters;
			}

			inline bool operator>( const reflector_zipper<size>& rhs )
			{
				return iters>rhs.iters;
			}
			inline bool operator<( const reflector_zipper<size>& rhs )
			{
				return iters<rhs.iters;
			}
		};

		template<typename T>
		struct reflector_wrapper
		{
			T& ref;
			reflector_wrapper( T& ref_ ) :ref{ ref_ } { }
			reflector_wrapper( reflector_wrapper&& ) = delete;
			reflector_wrapper( const reflector_wrapper& ) = delete;
			reflector_wrapper& operator=( const reflector_wrapper& ) = delete;
			reflector_wrapper& operator=( reflector_wrapper&& ) = delete;

			inline reflector_wrapper&& operator~()&&{ return std::move( *this ); }

			inline auto operator-()&&-> decltype( ref.reflect_as_tuple() )
			{ return ref.reflect_as_tuple(); }
		};

		template<typename T , disable_if<not_t<is_reflectable<rem_ref<T>>>> = 0>
		inline reflector_wrapper<rem_ref<T>> operator~( T& ref )
		{ return reflector_wrapper<rem_ref<T>>( ref ); }

		template<typename T>
		inline reflector_wrapper<rem_ref<T>> make_reflector_wrapper( T& ref )
		{ return reflector_wrapper<rem_ref<T>>( ref ); }
	}

	using utils::reflection::mirror;
	using utils::reflection::operator~;
}
namespace std
{
	template<size_t size>
	struct iterator_traits<::utils::reflection::reflector_zipper<size>>
	{
		using iterator_category = typename ::utils::reflection::reflector_zipper<size>::iterator_category;
		using value_type = typename ::utils::reflection::reflector_zipper<size>::value_type;
		using reference = typename ::utils::reflection::reflector_zipper<size>::reference;
	};

	inline void swap(
		::utils::reflection::reflected_variable_ref& lhs ,
		::utils::reflection::reflected_variable_ref& rhs )
	{
		lhs.swap( rhs );
	}
}

#define CONCATENATE_2(x,y) x##y
#define REM(...) __VA_ARGS__
#define REM_C(...) __VA_ARGS__ CONCATENATE_2(/,*)
#define EAT(...)

#define KEEP_TYPE(x) REM_C x */
#define REMOVE_TYPE(x) EAT x
#define REMOVE_PAREN(x) REM x

#define NAMEOFHHHHHHHH(x) #x
#define NAMEOFHHHHHHH(x)  NAMEOFHHHHHHHH(x)
#define NAMEOFHHHHHH(x)   NAMEOFHHHHHHH(x)
#define NAMEOFHHHHH(x)    NAMEOFHHHHHH(x)
#define NAMEOFHHHH(x)     NAMEOFHHHHH(x)
#define NAMEOFHHH(x)      NAMEOFHHHH(x)
#define NAMEOFHH(x)       NAMEOFHHH(x)
#define NAMEOFH(x)        NAMEOFHH(x)
#define NAMEOF(x)         NAMEOFH(x)

#define AS_VAR_DECL(r, data, i, elem) REM elem;
#define AS_STR_PARAM(r, data, i, elem) \
	BOOST_PP_COMMA_IF(i) NAMEOF( EAT elem )
#define AS_PARAM(r, data, i, elem) BOOST_PP_COMMA_IF(i) EAT elem

#if _HAS_CPP14
#define DEFINE_REFLECT_AS_TUPLE(seq)\
auto reflect_as_tuple()\
{ return std::tie( BOOST_PP_SEQ_FOR_EACH_I( AS_PARAM , null , seq ) ); }
#else
#define DEFINE_REFLECT_AS_TUPLE(seq)\
auto reflect_as_tuple()->\
	decltype(std::tie( BOOST_PP_SEQ_FOR_EACH_I( AS_PARAM , null , seq ) ))\
{ return std::tie( BOOST_PP_SEQ_FOR_EACH_I( AS_PARAM , null , seq ) ); }
#endif // _HAS_CPP14

#define UTILS_REFLECT_VARS_HELPER(seq) \
public:\
	DEFINE_REFLECT_AS_TUPLE(seq)\
	::utils::reflection::reflector reflect_all(){\
		constexpr auto seq_size = BOOST_PP_SEQ_SIZE(seq);\
		static ::std::array<::std::string , seq_size> str_init = \
			{BOOST_PP_SEQ_FOR_EACH_I(AS_STR_PARAM, null, seq)};\
		static bool is_trimmed = false;\
		if ( !is_trimmed ){\
			for ( auto& str : str_init )str = ::utils::trim( str );\
			is_trimmed = true;}\
		::std::array<::utils::reflection::reflected_variable_ref, seq_size> obj_ref\
			{BOOST_PP_SEQ_FOR_EACH_I(AS_PARAM, null, seq)};\
		return ::utils::reflection::reflector(\
					::utils::reflection::reflector_zipper<seq_size>(\
						::std::make_pair(str_init.begin(), \
						::std::make_move_iterator(obj_ref.begin()))),\
					::utils::reflection::reflector_zipper<seq_size>(\
						::std::make_pair(str_init.end(), \
						::std::make_move_iterator(obj_ref.end()))),\
					seq_size );}\
	inline auto operator~( ) ->\
		decltype(::utils::reflection::make_reflector_wrapper(*this))\
	{ return ::utils::reflection::make_reflector_wrapper(*this); }\

#define UTILS_DECL_AND_REFLECT_VARS_HELPER(seq) \
	BOOST_PP_SEQ_FOR_EACH_I(AS_VAR_DECL, null, seq) UTILS_REFLECT_VARS_HELPER(seq)

#define UTILS_DECL_AND_REFLECT_VARS(...)\
	UTILS_DECL_AND_REFLECT_VARS_HELPER(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

#define UTILS_REFLECT_VARS(...)\
	UTILS_REFLECT_VARS_HELPER(BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
