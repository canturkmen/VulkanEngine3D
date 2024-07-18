#pragma once

#include "defines.h"

// Disable assertions by commenting out the below line.
#define VEASSERTIONS_ENABLED

#ifdef VEASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

__declspec(dllexport) void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line);

#define VEASSERT(expr)                                                        \
    {                                                                         \
        if (expr) {                                                           \
        } else {                                                              \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);          \
            debugBreak();                                                     \
        }                                                                     \
    }                

#define VEASSERT_MSG(expr, message)                                           \
    {                                                                         \
        if (expr) {                                                           \
        } else {                                                              \
            report_assertion_failure(#expr, message, __FILE__, __LINE__);     \
            debugBreak();                                                     \
        }                                                                     \
    }                       

#ifdef _DEBUG
#define VEASSERT_DEBUG(expr)                                                  \
    {                                                                         \
        if (expr) {                                                           \
        } else {                                                              \
            report_assertion_failure(#expr, "", __FILE__, __LINE__);          \
            debugBreak()                                                      \
        }                                                                     \
    }            
#else
#define VEASSSERT_DEBUG(expr) // Does nothing at all.
#endif

#else
#define VEASSERT(expr)               // Does nothing at all.
#define VEASSERT_MSG(expr, message)  // Does nothing at all.
#define VEASSERT_DEBUG(expr)        // Does nothing at all.
#endif
