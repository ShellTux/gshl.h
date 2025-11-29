// gshl-priority: 99
#ifndef INCLUDE_MACROS_MOD_H_
#define INCLUDE_MACROS_MOD_H_

#include <stdio.h> // IWYU pragma: keep

#ifndef GSHLDEF
#    define GSHLDEF
#endif

#define GSHL_UNUSED(X) (void)(X)
#define GSHL_EXIT(...)                                                         \
    do {                                                                       \
        fprintf(stderr, "%s:%d: ", __FILE__, __LINE__);                        \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        exit(1);                                                               \
    } while (0)
#define GSHL_TODO(...) GSHL_EXIT("TODO: " __VA_ARGS__)
#define GSHL_UNREACHABLE(...) GSHL_EXIT("UNREACHABLE: " __VA_ARGS__)
#define GSHL_LOOP for (;;)
#ifdef GSHL_DEBUG
#    define GSHL_ASSERT(...) assert(__VA_ARGS__)
#else
#    define GSHL_ASSERT(...) GSHL_UNUSED((__VA_ARGS__))
#endif
#define GSHL_unlikely(CONDITION) __glibc_unlikely(CONDITION)
// Credits to RFGW
#define GSHL_MULTILINE_STR(...) #__VA_ARGS__

// Credit: https://gist.github.com/Jomy10/e0a8d34d3b5793a0d95524b1c5d5c143

#define GSHL_MACRO_ARGS_COUNT(...)                                             \
    GSHL_MACRO_ARGS_COUNT_(                                                    \
        __VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87,  \
        86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70,    \
        69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53,    \
        52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36,    \
        35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,    \
        18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define GSHL_MACRO_ARGS_COUNT_(                                                \
    _100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87,     \
    _86, _85, _84, _83, _82, _81, _80, _79, _78, _77, _76, _75, _74, _73, _72, \
    _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61, _60, _59, _58, _57, \
    _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, \
    _41, _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, \
    _26, _25, _24, _23, _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, \
    _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...)                      \
    N

#define GSHL_MACRO_CONCATENATION2(A, B) GSHL_MACRO_CONCATENATION2_(A, B)
#define GSHL_MACRO_CONCATENATION2_(A, B) A##B
#define GSHL_MACRO_CONCATENATION3(A, B, C) GSHL_MACRO_CONCATENATION3_(A, B, C)
#define GSHL_MACRO_CONCATENATION3_(A, B, C) A##B##C

#define GSHL_MACRO_FOREACH(ACTION, ...)                                        \
    GSHL_MACRO_CONCATENATION2(GSHL_MACRO_FOREACH_,                             \
                              GSHL_MACRO_ARGS_COUNT(__VA_ARGS__))(ACTION,      \
                                                                  __VA_ARGS__)

#define GSHL_MACRO_FOREACH_0(ACTION, ELEMENT) ELEMENT
#define GSHL_MACRO_FOREACH_1(ACTION, ELEMENT) ACTION(ELEMENT)
#define GSHL_MACRO_FOREACH_2(ACTION, ELEMENT, ...)                             \
    ACTION(ELEMENT) GSHL_MACRO_FOREACH_1(ACTION, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_3(ACTION, ELEMENT, ...)                             \
    ACTION(ELEMENT) GSHL_MACRO_FOREACH_2(ACTION, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_4(ACTION, ELEMENT, ...)                             \
    ACTION(ELEMENT) GSHL_MACRO_FOREACH_3(ACTION, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_5(ACTION, ELEMENT, ...)                             \
    ACTION(ELEMENT) GSHL_MACRO_FOREACH_4(ACTION, __VA_ARGS__)

#define GSHL_MACRO_FOREACH_N2(ACTION, EXTRA_VAR, ...)                          \
    GSHL_MACRO_CONCATENATION3(GSHL_MACRO_FOREACH_,                             \
                              GSHL_MACRO_ARGS_COUNT(__VA_ARGS__),              \
                              _N2_E)(ACTION, EXTRA_VAR, __VA_ARGS__)

#define GSHL_MACRO_FOREACH_0_N2_E(ACTION, ExtraVar, E, B) ExtraVar, E, B
#define GSHL_MACRO_FOREACH_2_N2_E(ACTION, EV, E, B) ACTION(EV, E, B)
#define GSHL_MACRO_FOREACH_4_N2_E(ACTION, EV, E, B, ...)                       \
    ACTION(EV, E, B) GSHL_MACRO_FOREACH_2_N2_E(ACTION, EV, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_6_N2_E(ACTION, EV, E, B, ...)                       \
    ACTION(EV, E, B) GSHL_MACRO_FOREACH_4_N2_E(ACTION, EV, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_8_N2_E(ACTION, EV, E, B, ...)                       \
    ACTION(EV, E, B) GSHL_MACRO_FOREACH_6_N2_E(ACTION, EV, __VA_ARGS__)
#define GSHL_MACRO_FOREACH_10_N2_E(ACTION, EV, E, B, ...)                      \
    ACTION(EV, E, B) GSHL_MACRO_FOREACH_8_N2_E(ACTION, EV, __VA_ARGS__)


#ifdef GSHL_STRIP_PREFIX
#    define ASSERT GSHL_ASSERT
#    define loop GSHL_LOOP
#    define LOOP GSHL_LOOP
#    define TODO GSHL_TODO
#    define UNREACHABLE GSHL_UNREACHABLE
#    define UNUSED GSHL_UNUSED
#    define unlikely GSHL_unlikely
#endif

#endif // INCLUDE_MACROS_MOD_H_
