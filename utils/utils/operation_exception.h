#pragma once
#ifndef __UTILITIES__OPERATION_EXCEPTION__
#define __UTILITIES__OPERATION_EXCEPTION__
#include <exception>
#include <string>
namespace utils
{
	enum class from;
	enum class error_type;
}
namespace std
{
	inline string to_string( utils::from );
	inline string to_string( utils::error_type );
}
namespace utils
{
	enum class from
	{
		static_array ,
		object,
		generic
	};

	enum class error_type
	{
		invalidIndex ,
		invalidObjectCast ,
		runtimeFunctionDeletion
	};

	class operation_exception :std::exception
	{
	private:
		const from _from;
		const error_type typeOfError;
		const std::string _what;
	public:
		using base = std::exception;
		inline operation_exception( const from& _from ,
									const error_type& errorType ) :_what( std::string( "Exception in the class \"" ) + std::to_string( _from ) + "\" with exception of type \"" + std::to_string( errorType ) + "\"." ) , _from( _from ) , typeOfError( errorType ) { }
		inline const from get_thrower() { return _from; }
		inline const error_type get_type() { return typeOfError; }
		virtual char const* what() const noexcept override
		{ return _what.c_str(); }
	};

	struct invalid_index: public operation_exception
	{
		inline invalid_index(const from& _from): 
			operation_exception(_from, error_type::invalidIndex){}
	};

	struct invalid_object_cast: public operation_exception
	{
		inline invalid_object_cast(): 
			operation_exception(from::object, error_type::invalidIndex)
		{}
	};

	struct runtime_function_deletion: public operation_exception
	{
		inline runtime_function_deletion(): 
			operation_exception(from::generic, error_type::runtimeFunctionDeletion){}
	};
}
namespace std
{
	string to_string( utils::from _Val )
	{
		switch ( _Val )
		{
		case utils::from::static_array:
			return "static_array";
		case utils::from::object:
			return "object";
		default:
			return "?";
		}
	}
	string to_string( utils::error_type _Val )
	{
		switch ( _Val )
		{
		case utils::error_type::invalidIndex:
			return "invalidIndex";
		case utils::error_type::invalidObjectCast:
			return "invalidObjectCast";
		case utils::error_type::runtimeFunctionDeletion:
			return "runtimeFunctionDeletion";
		default:
			return "?";
		}
	}
}
#endif // !__UTILITIES__OPERATION_EXCEPTION__
