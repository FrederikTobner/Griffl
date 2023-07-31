#ifndef COMMON_H_
#define COMMON_H_

#include <stdlib.h>

/// Allocates the memory for a single value of the specified type on the heap
#define new(type) (malloc(sizeof(type)))

#endif /* COMMON_H_ */