#pragma once

#include "defines.h"

__declspec(dllexport) u64 string_length(const char* str);

__declspec(dllexport) char* string_duplicate(const char* str);

// Case-sensitive string comparison. True if the same, otherwise false.
__declspec(dllexport) b8 string_equal(const char* str0, const char* str1);