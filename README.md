# utils
A set of varied utilities for C++.
The utilities.h header file contains multiple using type-alias like uint for unsigned int,
real for long doubleto shorten typing.

And it also contains functions for random number generationlike, 
rand_longto get a random value of type long long,
orrand_initto seed the random value generator based on the current timestamp,
and shuffle_collection to shuffle any collection, that overloads the array subscript ([]) operator, etc.

And some structs that delegate to operators, and some mathematical functions like abs to get the absolute value of an integral or floating-point type or floor to convert a floating-point type to an integral type by removing the digits after the floating-point.

Along withtype-traits like 
rem_ref, rem_const, etc. 
which remove references, constqualifiers, etc from types, and other type traits like 
is_ref, is_lval_ref, etc 
to check for certain attributes on types, 
or the select_tto select a type if a condition evaluates to true.

The iterator_addons.hheader file contains
the classes rec_range paired with rec_iteratorwhich are best described and explained in my youtube video channel.

And the class iterator_pairto pair begin and end iterators such that iterator_pair::begin() returns the first iterator and iterator_pair::end() returns the second iterator.

And a class, reverse_iterator_adapterthatmaps rbegin to begin and rend to endand the function reverse_adapt makes for easy construction of this class.

Along with a transform_iffunctionbest described and explained in my youtube video channel.

And multiple using type-alias related to std::iterator_traits to shorten typing.

The object.h header file contains a class called object that can hold a value of any type like a void* and it also occupies as-much-as space, on the heap (or free-store) and on the stack, as a void*.

The function obj_cast returns a reference/pointer to the value held in the object.

The type to be held in the object has to be copy-constructable.
The function_addons.hheader file contains functions that return function objects that work with functions e.g.
The func_chain can be used like
func_chain(f,g,h)(1,2,3) which is equal to
f(g(h(1,2,3)))
but func_chain, one might say, can be easilybe replicated with a lambda 
([](int a, int b, int c)
{return f(g(h(a, b, c)));}),
but that’s unlike multicallwhich can be used like
multicall(f,g,h)(1,2,3) which is equal to
f(1,2,3);
g(1,2,3);
h(1,2,3);
but that’s harder to represent with a lambda.
The returnedfunctor (function object) from multicall and func_chain can be stored in an auto variable or an std::function object.
Then there is the function for_each_valuewhich calls a function for a set of values. I also introduce utils::split which can split an std::string at any occurrence of a delimiter std::stringand save the split std::stringsto an output iterator (by the way I switched std::string tostd::basic_string in the next session).
