#include "darray.h"

#include "core/vememory.h"

__declspec(dllexport) void* _darray_create(u64 length, u64 stride);
__declspec(dllexport) void _darray_destroy(void* array);

__declspec(dllexport) u64 _darray_field_get(void* array, u64 field);
__declspec(dllexport) void _darray_field_set(void* array, u64 field, u64 value);

__declspec(dllexport) void* _darray_resize(void* array);

__declspec(dllexport) void* _darray_push(void* array, const void* value_ptr);
__declspec(dllexport) void _darray_pop(void* array, void* dest);

__declspec(dllexport) void* _darray_pop_at(void* array, u64 index, void* dest);
__declspec(dllexport) void* _darray_insert_at(void* array, u64 index, void* value_ptr);