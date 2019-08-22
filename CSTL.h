#ifndef __C_STL_H__
#define __C_STL_H__

#include <stdlib.h>
#include <string.h>

// Source: https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
/*****************************************************************************/
/* This header file allow for macro overloading based on the argument count. */
/* Usage:                                                                    */
/* #define <FUNCTION_NAME> VFUNC(<FUNCTION_IMPLEMENTATION_NAME>, __VA_ARGS__)*/
/*****************************************************************************/

#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

// general definition for any function name
#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

/*****************************************************************************/
/* Vector implementation                                                     */
/*****************************************************************************/

/*****************************************************************************/
/* Helper functions. Internal use only.                                      */
/*****************************************************************************/
static int GetAllignedValue(int value, int allignment) {
	return (((value / allignment) + 1) * allignment);
}

#define __default_compare(a, b) (a > b)

/*****************************************************************************/
/* Used to determine the template type of a vector_t structure               */
/*****************************************************************************/
#define _T(this) typeof(*(*this).data)

#define VECTOR_DEFAULT_CAPACITY	8

#define vector_t(T) struct {                                                  \
	size_t size;                                                          \
	size_t capacity;                                                      \
	T* data;                                                              \
}

#define new_vector_2(T, cap)                                                  \
{                                                                             \
	(const size_t)0,                                                      \
	(const size_t)(cap),                                                  \
	(T*)((cap) > 0) ? malloc(sizeof(T) * (cap)) : NULL                    \
}

#define new_vector_1(T) new_vector_2(T, VECTOR_DEFAULT_CAPACITY)
/*****************************************************************************/
/* Helper functions. Allows for function overloading.                        */
/* Can be used with or without a specific capacity                           */
/*****************************************************************************/
#define new_vector(...) VFUNC(new_vector_, __VA_ARGS__)

#define vector_reserve(this, parameter_1)                                     \
{                                                                             \
	const size_t capacity = parameter_1;                                  \
	if ((this)->capacity < capacity) {                                    \
		_T(this)* newData = (_T(this)*)malloc(capacity);              \
		memcpy(newData, (this)->data, (this)->size);                  \
		free((this)->data);                                           \
		(this)->capacity = capacity;                                  \
		(this)->data = newData;                                       \
	}                                                                     \
}

/*****************************************************************************/
/* Initialize an object of type vector_t with a specific capacity reserved   */
/*****************************************************************************/
#define vector_init_2(this, parameter_1)                                      \
{                                                                             \
	const size_t capacity = parameter_1;                                  \
	(this)->size = 0;                                                     \
	(this)->capacity = parameter_1;                                       \
	if ((this)->capacity > 0) {                                           \
		(this)->data = (_T(this)*)malloc(                             \
				sizeof(_T(this)) * (this)->capacity);         \
	}                                                                     \
}                                                                             \

/*****************************************************************************/
/* Initialize an object of type vector_t with a default(8) capacity reserved */
/*****************************************************************************/
#define vector_init_1(this) vector_init_2((this), VECTOR_DEFAULT_CAPACITY);

/*****************************************************************************/
/* Helper functions. Allows for function overloading.                        */
/* Can be used with or without a specific capacity                           */
/*****************************************************************************/
#define vector_init(...) VFUNC(vector_init_, __VA_ARGS__)

/*****************************************************************************/
/* Free up memory used internally by the vector_t object                     */
/*****************************************************************************/
#define vector_uninit(this)                                                   \
{                                                                             \
	(this)->size = 0;                                                     \
	(this)->capacity = 0;                                                 \
	if (((this)->data)) free((this)->data);                               \
	(this)->data = NULL;                                                  \
}

/*****************************************************************************/
/* Returns the number of entryes in the vector                               */
/*****************************************************************************/
#define vector_size(this) ((const size_t)(this)->size)

/*****************************************************************************/
/* Returns the reserved memory size                                          */
/*****************************************************************************/
#define vector_capacity(this) ((const size_t)(this)->capacity)

/*****************************************************************************/
/* Returns the entry found at a specific index                               */
/*****************************************************************************/
//#define vector_get(this, index) ((const _T(this))(this)->data[(index)])
#define vector_get(this, index) ((this)->data[(index)])

/*****************************************************************************/
/* Insert an array of the same type into the vector                          */
/*****************************************************************************/
#define vector_insert_3(this, parameter_1, parameter_2)                       \
{                                                                             \
	const _T(this)* begin = (parameter_1);                                \
	const _T(this)* end = (parameter_2);                                  \
	const size_t length = (end) - (begin);                                \
	const size_t newSize = (this)->size + length;                         \
	int i;                                                                \
	if (newSize >= (this)->capacity)                                      \
	{                                                                     \
		const size_t allignedValue = GetAllignedValue(newSize, 8);    \
		_T(this)* newData = malloc(sizeof(_T(this)) * allignedValue); \
		(this)->capacity = allignedValue;                             \
		for (i = 0; i < newSize; ++i)                                 \
		{                                                             \
			newData[i] = (i < (this)->size) ?                     \
				(this)->data[i] : (begin)[i - (this)->size];  \
		}                                                             \
		free((this)->data);                                           \
		(this)->data = newData;                                       \
	}                                                                     \
	else                                                                  \
	{								      \
		for (i = (this)->size; i < newSize; ++i)                      \
		{                                                             \
			(this)->data[i] = (begin)[i - (this)->size];          \
		}                                                             \
	}                                                                     \
	(this)->size = newSize;                                               \
}

/*****************************************************************************/
/* Insert other vector data into this vector.                                */
/*****************************************************************************/
#define vector_insert_2(this, other)                                          \
              vector_insert_3((this), vector_begin(other), vector_end(other));

/*****************************************************************************/
/* Helper functions. Allows for function overloading.                        */
/* Can be used a raw array or another vector.                                */
/*****************************************************************************/
#define vector_insert(...) VFUNC(vector_insert_, __VA_ARGS__);

/*****************************************************************************/
/* Removes a number of consecutive entryes from the vector                   */
/*****************************************************************************/
#define vector_remove(this, parameter_1, parameter_2)                         \
{                                                                             \
	const size_t begin = (parameter_1);                                   \
	const size_t count = (parameter_2);                                   \
	if ((count) > 0) {                                                    \
		int i;                                                        \
		const int newSize = (this)->size - (count);                   \
		if (newSize >= 0) {                                           \
			for (i = 0; i < (count); ++i)                         \
			{                                                     \
				(this)->data[(begin) + i] =                   \
					(this)->data[(begin) + (count) + i];  \
			}                                                     \
			(this)->size = newSize;                               \
		}                                                             \
	}                                                                     \
}

#define vector_push_back(this, parameter_1)                                   \
{                                                                             \
	const _T(this) value = (parameter_1);                                 \
	const _T(this)* pvalue = &value;                                      \
	vector_insert((this), pvalue, pvalue + 1);                            \
}

#define vector_pop_back(this, parameter_1)                                    \
{                                                                             \
	const _T(this)* value = (parameter_1);                                \
	if ((this)->size == 0) {                                              \
		*(value) = (_T(this))0;                                       \
	}                                                                     \
	else                                                                  \
	{                                                                     \
		*(value) = (this)->data[(this)->size - 1];                    \
		vector_remove((this), (this)->size - 1, 1);                   \
	}                                                                     \
}

/*****************************************************************************/
/* Set an entry value by index                                               */
/*****************************************************************************/
#define vector_set(this, index, value)                                        \
{                                                                             \
	if ((index) < (this)->size)                                           \
	{                                                                     \
		(this)->data[(index)] = (_T(this))(value);                    \
	}                                                                     \
}

#define vector_begin(this) ((_T(this)*)((this)->data))

#define vector_cbegin(this) ((const _T(this)*)((this)->data))

#define vector_end(this) ((_T(this)*)(&((this)->data[(this)->size])))

#define vector_cend(this) ((const _T(this)*)(&((this)->data[(this)->size])))

#define vector_clear(this) { (this)->size = 0; }

#define vector_sort_2(this, function)                                         \
{                                                                             \
	int loop = 0;                                                         \
	int index = 0;                                                        \
	_T(this) tmp;                                                         \
	do                                                                    \
	{                                                                     \
		loop = 0;                                                     \
		for (index = 0; index < vector_size((this)) - 1; ++index) {   \
			if (function(vector_get((this), index),               \
					vector_get((this), index + 1))) {     \
				tmp = vector_get((this), index + 1);          \
				vector_set((this), index + 1,                 \
						vector_get((this), index));   \
				vector_set((this), index, tmp);               \
				loop = 1;                                     \
			}                                                     \
		}                                                             \
	} while (loop);                                                       \
}

#define vector_sort_1(this) vector_sort_2(this, __default_compare)

#define vector_sort(...) VFUNC(vector_sort_, __VA_ARGS__);

/*****************************************************************************/
/* A foreach implementation for the vector_t structure.                      */
/* Calls the compound statement "body" for every entity in the vector.       */
/*****************************************************************************/
#define vector_foreach(this, iter, body)                                      \
	for (_T(this)* (iter) = vector_begin(this);                           \
             iter != vector_end(this);	++iter)	{body;}

/*****************************************************************************/
/* String implementation                                                     */
/*****************************************************************************/

typedef vector_t(char) string;

#define new_string_0() new_vector_1(char)

#define new_string_1(scap) new_vector_2(char, 8)

#define new_string(...) VFUNC(new_string_, __VA_ARGS__);

#define string_init(this) vector_init(this)

#define string_uninit(this) vector_uninit(this)

#define string_c_str(this) ((const char*)(this)->data)

#define string_clear(this) vector_clear(this)

#define string_assign_int(this, value)                                        \
{                                                                             \
	const int parameter_1 = value;                                        \
                                                                              \
	static char tmp[128];                                                 \
	itoa(parameter_1, tmp, 10);                                           \
	string_assign_string((this), tmp);                                    \
}

#define string_assign_float(this, value)                                      \
{                                                                             \
	const float parameter_1 = value;                                      \
                                                                              \
	static char tmp[128];                                                 \
	ftoa(parameter_1, tmp, 10);                                           \
	string_assign_string((this), tmp);                                    \
}

#define string_assign_string(this, value)                                     \
{                                                                             \
	const char* parameter_1 = value;                                      \
	const unsigned int parameter_1_length = strlen(parameter_1);          \
	vector_clear(this);                                                   \
	vector_insert((this), parameter_1,                                    \
                                        parameter_1 + parameter_1_length + 1);\
}

#define string_assign(this, value)                                            \
{                                                                             \
/*
	vector_clear(this);                                                   \
	_Generic((value),                                                     \
		int   : (string_assign_int((this), (value))),                 \
		float : (string_assign_float((this), (value))),               \
		char* : (string_assign_string((this), (value)))               \
		);                                                            \
	vector_insert((this), value, value + strlen(value) + 1);              \
*/\
	string_assign_string(this, value);                                    \
}

#define string_compare(this, value) (strcmp(string_c_str(this), (value)))

#define string_equal(this, value) (string_compare((this), (value)) == 0)

#define string_size(this) (vector_size(this))

#define string_length(this) ((const size_t)strlen((this)->data))

#define string_capacity(this) (vector_capacity(this))

#define string_to_int(this) (atoi(string_c_str(this)))

#define string_to_float(this) (atof(string_c_str(this)))

#define string_append(this, value)                                            \
{                                                                             \
	const char* parameter_1 = value;                                      \
	const unsigned int parameter_1_length = strlen(parameter_1);          \
	vector_reserve((this), (this)->size + parameter_1_length);            \
	(this)->data = strcat((this)->data, parameter_1);                     \
	(this)->size += parameter_1_length;                                   \
}

#endif //__C_STL_H__
