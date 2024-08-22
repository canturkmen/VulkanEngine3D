#include "vestring.h"
#include "core/vememory.h"

#include <stdio.h> 
#include <string.h>
#include <stdarg.h>

u64 string_length(const char* str)
{
    return strlen(str);
} 

char* string_duplicate(const char* str)
{
    u64 length = string_length(str);
    char* copy = veallocate(length + 1, MEMORY_TAG_STRING);
    vecopy_memory(copy, str, length + 1);
    return copy;
}

b8 string_equal(const char* str0, const char* str1)
{
    return strcmp(str0, str1) == 0;
}

VEAPI i32 string_format(char* dest, const char* format, ...)
{
    if(dest)
    {
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, format);
        i32 written = string_format_v(dest, format, arg_ptr);
        va_end(arg_ptr);
        return written;
    }
    
    return -1;
}

VEAPI i32 string_format_v(char* dest, const char* format, void* va_listp)
{
    if(dest)
    {
        // Big, but can fit on the stack.
        char buffer[32000];
        i32 written = vsnprintf(buffer, 32000, format, va_listp);
        buffer[written] = 0;
        vecopy_memory(dest, buffer, written + 1);

        return written;
    }

    return -1;
}

