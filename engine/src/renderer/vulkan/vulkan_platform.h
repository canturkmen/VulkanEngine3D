#pragma once

#include "defines.h"

b8 platform_create_vulkan_surface(struct vulkan_context* context);

/**
 * Appends the names of required extensions for this platform to
 * the names_daray, which should be created and passed in.
*/
void platform_get_required_extension_names(const char*** names_darray);