#ifndef __STRING_H__
#define __STRING_H__

#include <string.h>

#include "Vector.h"

typedef vector_t(char) string;

#define new_string_0() new_vector_1(char)

#define new_string_1(scap) new_vector_2(char, 8)

#define new_string(...) VFUNC(new_string_, __VA_ARGS__);

#define string_init(this) vector_init(this)

#define string_uninit(this) vector_uninit(this)

#define string_c_str(this) ((const char*)(this)->data)

#define string_clear(this) vector_clear(this)

#define string_assign(this, value)                                            \
{                                                                             \
	vector_reserve((this), strlen(value) + 1);                            \
	strcpy((this)->data, (value));                                        \
}

#define string_compare(this, value) (strcmp(string_c_str(this), (value)))

#define string_equal(this, value) (string_compare((this), (value)) == 0)

#define string_size(this) (vector_size(this))

#define string_capacity(this) (vector_capacity(this))

#define string_to_int(this) (atoi(string_c_str(this)))

#define string_to_float(this) (atof(string_c_str(this)))

#define string_concat(this, value) vector_insert((this), (value),             \
						(value) + strlen(value))

#endif //__STRING_H__
