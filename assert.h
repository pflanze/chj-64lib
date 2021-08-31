#ifndef ASSERT_H_
#define ASSERT_H_

#include <assert.h>

#ifndef RELEASE
# define ASSERT(x) assert(x)
#else
# define ASSERT(x)
#endif

#endif /* ASSERT_H_ */
