#include "vememory.h"

#include "core/logger.h"
#include "platform/platform.h"
#include "core/vestring.h"

// TODO: Custom string lib.
#include <string.h>
#include <stdio.h>

static const char* memory_tag_strings[MEMORY_TAG_MAX_TAGS] =
{
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      ",  
};

struct memory_stats
{
    u64 total_allocated;
    u64 tagged_allocation[MEMORY_TAG_MAX_TAGS];
};

static struct memory_stats stats;

void initialize_memory()
{
    platform_zero_memory(&stats, sizeof(stats));
}

void shutdown_memory()
{

}

void* veallocate(u64 size, memory_tag tag)
{
    if(tag == MEMORY_TAG_UNKNOWN)
        VEWARN("veallocate called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");

    stats.total_allocated += size;   
    stats.tagged_allocation[tag] += size;

    // TODO: Memory alignment.
    void* block = platform_allocate(size, FALSE);
    platform_zero_memory(block, size);
    return block;
}

void vefree(void* block, u64 size, memory_tag tag)
{
    if(tag == MEMORY_TAG_UNKNOWN)
        VEWARN("vefree called using MEMORY_TAG_UNKNOWN. Re-class this allocation.");
    
    stats.total_allocated -= size;
    stats.tagged_allocation[tag] -= size;

    // TODO: Memory alignment.
    platform_free(block, FALSE);
}

void* vezero_memory(void* block, u64 size)
{
    return platform_zero_memory(block, size);
}

void* vecopy_memory(void* dest, const void* source, u64 size)
{
    return platform_copy_memory(dest, source, size);
}

void* veset_memory(void* dest, i32 value, u64 size)
{
    return platform_set_memory(dest, value, size);
}

char* get_memory_usage_str()
{
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    char buffer[8000] = "System memory usage (tagged):\n";
    u64 offset = strlen(buffer);
    for(u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i)
    {
        char unit[4] = "XiB";
        float amount = 1.0f;
        if(stats.tagged_allocation[i] >= gib) 
        {
            unit[0] = 'G';
            amount = stats.tagged_allocation[i] / (float)gib;
        }
        else if (stats.tagged_allocation[i] >= mib)
        {
            unit[0] = 'M';
            amount = stats.tagged_allocation[i] / (float)mib;
        }
        else if (stats.tagged_allocation[i] >= kib)
        {
            unit[0] = 'K';
            amount = stats.tagged_allocation[i] / (float)kib;
        }
        else
        {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)stats.tagged_allocation[i];
        }

        i32 length = snprintf(buffer + offset, 8000, "   %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
        offset += length;
    }

    char* out_string = string_duplicate(buffer);
    return out_string;
}