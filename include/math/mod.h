#ifndef INCLUDE_MATH_MOD_H_
#define INCLUDE_MATH_MOD_H_

#include "types/mod.h"

/// {{{ Functions

usize GSHL_factorial(const usize number);
bool GSHL_is_power_of_two(const usize number);

/// }}}

#ifdef GSHL_STRIP_PREFIX
#    define factorial GSHL_factorial
#    define is_power_of_two GSHL_is_power_of_two
#endif

#endif // INCLUDE_MATH_MOD_H_
