#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>
#include "VFunc.h"

/*****************************************************************************/
/* Helper functions. Internal use only.                                      */
/*****************************************************************************/
static int GetAllignedValue(int value, int allignment) {
	return (((value / allignment) + 1) * allignment);
}

/*****************************************************************************/
/* Used to determine the template type of a vector_t structure               */
/*****************************************************************************/
#define VEC_T(this) typeof(*(*this).data)

#define VECTOR_DEFAULT_CAPACITY	8

#define vector_t(T) struct {							\
	unsigned int size;							\
	unsigned int capacity;							\
	T* data;								\
}

#define vector_reserve(this, newcapacity)					\
{										\
	if ((this)->capacity < (newcapacity)) {					\
		VEC_T(this)* newData = (VEC_T(this)*)malloc((newcapacity));	\
		memcpy(newData, (this)->data, (this)->size);			\
		free((this)->data);						\
		(this)->capacity = (newcapacity);				\
		(this)->data = newData;						\
	}									\
}

/*****************************************************************************/
/* Initialize an object of type vector_t with a specific capacity reserved   */
/*****************************************************************************/
#define vector_init_2(this, pcapacity)						\
{										\
	(this)->size = 0;							\
	(this)->capacity = (pcapacity);						\
	if ((this)->capacity > 0) {						\
		(this)->data = (VEC_T(this)*)malloc(				\
				sizeof(VEC_T(this)) * (this)->capacity);	\
	}									\
}										\

/*****************************************************************************/
/* Initialize an object of type vector_t with a default(8) capacity reserved */
/*****************************************************************************/
#define vector_init_1(this) vector_init_2(this, VECTOR_DEFAULT_CAPACITY);

/*****************************************************************************/
/* Helper functions. Allows for function overloading.                        */
/* Can be used with or without a specific capacity                           */
/*****************************************************************************/
#define vector_init(...) VFUNC(vector_init_, __VA_ARGS__)

/*****************************************************************************/
/* Free up memory used internally by the vector_t object                     */
/*****************************************************************************/
#define vector_uninit(this)							\
{										\
	(this)->size = 0;							\
	(this)->capacity = 0;							\
	if (((this)->data)) free((this)->data);					\
	(this)->data = NULL;							\
}

/*****************************************************************************/
/* Returns the number of entryes in the vector                               */
/*****************************************************************************/
#define vector_size(this) ((const unsigned int)(this)->size)

/*****************************************************************************/
/* Returns the reserved memory size                                          */
/*****************************************************************************/
#define vector_capacity(this) ((const unsigned int)(this)->capacity)

/*****************************************************************************/
/* Returns the entry found at a specific index                               */
/*****************************************************************************/
#define vector_get(this, index) ((const VEC_T(this))(this)->data[index])

/*****************************************************************************/
/* Insert an array of the same type into the vector                          */
/*****************************************************************************/
#define vector_insert_3(this, begin, end)					\
{										\
	int i;									\
	const unsigned int length = (end) - (begin);				\
	const unsigned int newSize = (this)->size + length;			\
	if (newSize >= (this)->capacity) {					\
		unsigned int allignedValue = GetAllignedValue(newSize, 8);	\
		VEC_T(this)* newData = malloc(					\
					sizeof(VEC_T(this)) * allignedValue);	\
		(this)->capacity = allignedValue;				\
		for (i = 0; i < newSize; ++i) {					\
			newData[i] = (i < (this)->size) ? 			\
				(this)->data[i] : (begin)[i - (this)->size];	\
		}								\
		free((this)->data);						\
		(this)->data = newData;						\
	} else {								\
		for (i = (this)->size; i < newSize; ++i) {			\
			(this)->data[i] = (begin)[i - (this)->size];		\
		}								\
	}									\
	(this)->size = newSize;							\
}

/*****************************************************************************/
/* Insert other vector data into this vector.                                */
/*****************************************************************************/
#define vector_insert_2(this, other)						\
              vector_insert_3((this), vector_begin(other), vector_end(other));

/*****************************************************************************/
/* Helper functions. Allows for function overloading.                        */
/* Can be used a raw array or another vector.                                */
/*****************************************************************************/
#define vector_insert(...) VFUNC(vector_insert_, __VA_ARGS__);

/*****************************************************************************/
/* Removes a number of consecutive entryes from the vector                   */
/*****************************************************************************/
#define vector_remove(this, begin, count)					\
{										\
	if ((count) > 0) {							\
		int i;								\
		const int newSize = (this)->size - (count);			\
		if (newSize >= 0) {						\
			for (i = 0; i < (count); ++i) {				\
				(this)->data[(begin) + i] = 			\
					(this)->data[(begin) + (count) + i];	\
			}							\
			(this)->size = newSize;					\
		}								\
	}									\
}

#define vector_push_back(this, value) 						\
{										\
	const VEC_T(this) tmp = (value);						\
	const VEC_T(this)* ptmp = &tmp;						\
	vector_insert((this), ptmp, ptmp + 1);					\
}

#define vector_pop_back(this, value)						\
{										\
	if ((this)->size == 0) {						\
		*(value) = (VEC_T(this))0;					\
	} else {								\
		*(value) = (this)->data[(this)->size - 1];			\
		vector_remove((this), (this)->size - 1, 1);			\
	}									\
}

/*****************************************************************************/
/* Set an entry value by index                                               */
/*****************************************************************************/
#define vector_set(this, index, value)						\
{										\
	if ((index) < (this)->size) {						\
		(this)->data[(index)] = (VEC_T(this))(value);			\
	}		
}

#define vector_begin(this) ((const VEC_T(this)*)((this)->data))
#define vector_end(this) ((const VEC_T(this)*)(&((this)->data[(this)->size])))
#define vector_clear(this)							\
	{									\
	(this)->size = 0;							\
	}

#define vector_sort(this, compare)						\
	{									\
	int loop = 0;							\
	int index = 0;								\
	VEC_T(this) tmp;							\
	do {									\
		loop = 0;							\
		for (index = 0; index < vector_size((this)) - 1; ++index) {	\
			if ((compare)(vector_get((this), index),		\
					vector_get((this), index + 1))) {	\
				tmp = vector_get((this), index + 1);		\
				vector_set((this), index + 1, vector_get((this), index));\
				vector_set((this), index, tmp);			\
				loop = 1;					\
			}							\
		}								\
	} while (loop);								\
	}

#endif // __VECTOR_H__
