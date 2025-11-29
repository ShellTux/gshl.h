#ifndef INCLUDE_TAGGED_UNION_MOD_H_
#define INCLUDE_TAGGED_UNION_MOD_H_

#include "macros/mod.h"

#define __ENUM_NAME(NAME) NAME##Tag
#define __ENUM_VAL(NAME) __ENUM_NAME(NAME),
#define __UNION_FIELD(NAME) NAME __UNION_NAME(NAME);
#define __UNION_NAME(NAME) as##NAME

#define GSHL_TaggedUnionTypeDecl(NAME, ...)                                    \
    struct NAME {                                                              \
        enum NAME##Tag{GSHL_MACRO_FOREACH(__ENUM_VAL, __VA_ARGS__)} tag;       \
        union {                                                                \
            GSHL_MACRO_FOREACH(__UNION_FIELD, __VA_ARGS__)                     \
        };                                                                     \
    }

#define GSHL_TaggedUnionValue(TAG, ...)                                        \
    {                                                                          \
        .tag = __ENUM_NAME(TAG), .__UNION_NAME(TAG) = { __VA_ARGS__ }          \
    }

#define __MATCH_CASE(TU, TAG, block)                                           \
case __ENUM_NAME(TAG): {                                                       \
    const TAG *__UNION_NAME(TAG) = &(TU).__UNION_NAME(TAG);                    \
    block;                                                                     \
};

#define GSHL_match(TAGGED_UNION, ...)                                          \
    switch ((TAGGED_UNION).tag) {                                              \
        GSHL_MACRO_FOREACH_N2(__MATCH_CASE, TAGGED_UNION, __VA_ARGS__)         \
    default:                                                                   \
        GSHL_UNREACHABLE("Unknown tag: %u", (TAGGED_UNION).tag);               \
        break;                                                                 \
    }

#ifdef GSHL_STRIP_PREFIX
#    define TaggedUnionValue GSHL_TaggedUnionValue
#    define TaggedUnionTypeDecl GSHL_TaggedUnionTypeDecl
#    define TUValue GSHL_TaggedUnionValue
#    define TUTypeDecl GSHL_TaggedUnionTypeDecl
#    define match GSHL_match
#endif

#endif // INCLUDE_TAGGED_UNION_MOD_H_
