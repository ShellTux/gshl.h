#ifndef INCLUDE_INCLUDE_MATH_H_
#define INCLUDE_INCLUDE_MATH_H_

#include "types/mod.h"

/// {{{ Functions

usize GSHL_factorial(const usize number);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define factorial GSHL_factorial
#endif

#endif // INCLUDE_INCLUDE_MATH_H_
