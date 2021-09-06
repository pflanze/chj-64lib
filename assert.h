/*
  Copyright (C) 2021 Christian Jaeger, <ch@christianjaeger.ch>
  Published under the terms of the MIT License, see the LICENSE file.
*/
#ifndef ASSERT_H_
#define ASSERT_H_

#include <assert.h>

#ifndef RELEASE
# define ASSERT(x) assert(x)
#else
# define ASSERT(x)
#endif

#endif /* ASSERT_H_ */
