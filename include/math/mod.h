#ifndef INCLUDE_MATH_MOD_H_
#define INCLUDE_MATH_MOD_H_

#include "types/mod.h"

/// {{{ Functions

usize GSHL_factorial(const usize number);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define factorial GSHL_factorial
#endif

#endif // INCLUDE_MATH_MOD_H_
