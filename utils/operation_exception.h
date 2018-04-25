#pragma once
#ifndef __UTILITIES__DATA_STRUCTURE_EXCEPTION__
#define __UTILITIES__DATA_STRUCTURE_EXCEPTION__

namespace utils
{
	enum class from
	{
		stack ,
		queue ,
		double_ended_queue ,
		linked_list ,
		dynamic_array ,
		static_array,
		object
	};

	enum class error_type
	{
		overflow ,
		underflow ,
		underflowAtRear ,
		underflowAtFront ,
		valueNotPresent ,
		memoryOverflow ,
		indexTooLarge ,
		invalidObjectCast
	};

	class operation_exception
	{
	private:
		from dataStructure;
		error_type typeOfError;
	public:
		inline operation_exception( const from dataStructureError = from::stack ,
									const error_type dataStructureErrorType = error_type::underflow ) : dataStructure( dataStructureError ) , typeOfError( dataStructureErrorType ) { }
		inline const from get_thrower() { return dataStructure; }
		inline const error_type get_type() { return typeOfError; }
		inline ~operation_exception() { }
	};
}
#endif // !__UTILITIES__DATA_STRUCTURE_EXCEPTION__
