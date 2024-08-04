#pragma once

#include "defines.h"
#include "core/asserts.h"

#include <vulkan/vulkan.h>

// Checks the given expression's return value agains VK_SUCCESS.
#define VK_CHECK(expr)              \
    {                                   \
        VEASSERT(expr == VK_SUCCESS);   \
    }                                        

typedef struct vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;
#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debug_messenger; 
#endif
} vulkan_context;