#pragma once

#include "ExceptionImpl.hpp"

#define STRINGIFY(x) #x
#define TOSTRING(x)    STRINGIFY(x)


#define expect(expr_)                                   \
    do {                                                \
        bool b = (expr_);                               \
                                                        \
        if (b) break;                                   \
                                                        \
        throw mpisimulator::MisusageImpl(               \
            __FILE__ ":" TOSTRING(__LINE__) ": " #expr_ \
        );                                              \
                                                        \
    } while (false);

#define must(expr_)         \
    do {                    \
        bool b = (expr_);   \
                            \
        if (!b) abort();    \
                            \
    } while (false);
