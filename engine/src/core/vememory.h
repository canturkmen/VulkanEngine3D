#pragma once

#include "defines.h"

typedef enum memory_tag 
{
    // For temporary use. Should be assigned one of the below or have a new tag created.
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_MAX_TAGS
} memory_tag;

__declspec(dllexport) void initialize_memory();
__declspec(dllexport) void shutdown_memory();

__declspec(dllexport) void* veallocate(u64 size, memory_tag tag);

__declspec(dllexport) void vefree(void* block, u64 size, memory_tag tag);

__declspec(dllexport) void* vezero_memory(void* block, u64 size);

__declspec(dllexport) void* vecopy_memory(void* dest, const void* source, u64 size);

__declspec(dllexport) void* veset_memory(void* dest, i32 value, u64 size);

__declspec(dllexport) char* get_memory_usage_str();
