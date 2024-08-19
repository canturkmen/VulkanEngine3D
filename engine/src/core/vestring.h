#pragma once

#include "defines.h"

VEAPI u64 string_length(const char* str);

VEAPI char* string_duplicate(const char* str);

// Case-sensitive string comparison. True if the same, otherwise false.
VEAPI b8 string_equal(const char* str0, const char* str1);