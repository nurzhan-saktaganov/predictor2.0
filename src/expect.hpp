#pragma once

#include <cstdlib>

#include "ExceptionImpl.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x)    STRINGIFY(x)


#define _expect(expr_)                                  \
    do {                                                \
        if (expr_) break;                               \
                                                        \
        throw new dvmpredictor::MisusageImpl(           \
            __FILE__ ":" TOSTRING(__LINE__) ": " #expr_ \
        );                                              \
                                                        \
    } while (false);

#define _must(expr_)        \
    do {                    \
        if (expr_) break;   \
                            \
        abort();            \
                            \
    } while (false);

#define expect(x_)	_must(x_)
#define must(x_)	_must(x_)
