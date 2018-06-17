# utils
A set of varied utilities for C++.

utilities.h:
The utilities.h header file contains multiple using type-alias like uint for unsigned int,
real for long double to shorten typing.

And it also contains functions for random number generation like, 
rand_long to get a random value of type long long,
or rand_init to seed the random value generator based on the current timestamp,
and shuffle_collection to shuffle any collection, that overloads the array subscript ([]) operator, etc.

And some structs that delegate to operators, and some mathematical functions like 
abs to get the absolute value of an integral or floating-point type or 
floor to convert a floating-point type to an integral type by removing the digits after the floating-point.

Along with type-traits like 
rem_ref, rem_const, etc. 
which remove references, const qualifiers, etc from types, and other type traits like 
is_ref, is_lval_ref, etc 
to check for certain attributes on types, 
or the select_t to select a type if a condition evaluates to true.

iterator_addons.h:
The iterator_addons.h header file contains
the classes rec_range paired with rec_iterator which are best described and 
explained in my youtube video channel.

And the class iterator_pair to pair begin and end iterators 
such that iterator_pair::begin() returns the first iterator and 
iterator_pair::end() returns the second iterator.

And a class, reverse_iterator_adapter that maps 
rbegin to begin and 
rend to end and the function reverse_adapt makes for easy construction of this class.

Along with a transform_if function best described and explained in my youtube video channel.

And multiple using type-alias related to std::iterator_traits to shorten typing.

It also contains utils::split which can split an std::basic_string at any occurrence of a 
delimiter std::basic_string and save the split std::basic_string to an output iterator.

object.h:
The object.h header file contains a class called object that can hold a value of any type 
like a void* and it also occupies as-much-as space, on the heap (or free-store) 
and on the stack, as a void*.

The function obj_cast returns a reference/pointer to the value held in the object.

The type to be held in the object has to be copy-constructable.

function_addons.h:
The function_addons.h header file contains functions that 
return function objects or work with functions e.g.
The func_chain can be used like
func_chain(f,g,h)(1,2,3) which is equal to
f(g(h(1,2,3)))
but func_chain, one might say, can be easily be replicated with a lambda 
([](int a, int b, int c)
{return f(g(h(a, b, c)));}),
but that’s unlike multicall which can be used like
multicall(f,g,h)(1,2,3) which is equal to
f(1,2,3);
g(1,2,3);
h(1,2,3);
but that’s harder to represent with a lambda.
The returned functor (function object) from multicall and func_chain can be 
stored in an auto variable or an std::function object.
Then there is the function for_each_value which calls a function for a set of values. 
