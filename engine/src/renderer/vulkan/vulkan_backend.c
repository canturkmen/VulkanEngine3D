#include "vulkan_backend.h"

#include "vulkan_types.inl"

#include "core/logger.h"
#include "core/vestring.h"

#include "containers/darray.h"

#include "vulkan_platform.h"
#include "vulkan_device.h"

// Static Vulkan Context.
static vulkan_context context;

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data);

b8 vulkan_renderer_backend_initialize(struct renderer_backend* backend, const char* application_name, struct platform_state* plat_state)
{   
    // TODO: Custom allocator.
    context.allocator = 0;

    // Setup Vulkan instance.
    VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = application_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Vulkan Engine 3D";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
    create_info.pApplicationInfo = &app_info;

    // Obtain a list of required extensions.
    const char** required_extensions = darray_create(const char*);
    darray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);   // Generic surface extension.
    platform_get_required_extension_names(&required_extensions);        // Platform specific extensions.
#if defined(_DEBUG)
    darray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME); // Debug utilities.

    VEDEBUG("Required Extensions:");
    u32 length = darray_length(required_extensions);
    for(u32 i = 0; i < length; ++i)
    {
        VEDEBUG(required_extensions[i]);
    }
#endif

    create_info.enabledExtensionCount = darray_length(required_extensions);
    create_info.ppEnabledExtensionNames = required_extensions;

    // Validation layers.
    const char** required_validation_layer_names = 0;
    u32 required_validation_layer_count = 0;

#if defined(_DEBUG)
    VEINFO("Validation layers enabled. Enumerating...");

    // The list of validation layers required.
    required_validation_layer_names = darray_create(const char*);
    darray_push(required_validation_layer_names, &"VK_LAYER_KHRONOS_validation");
    required_validation_layer_count = darray_length(required_validation_layer_names);

    // Obtain a list of available validation layers.
    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties* available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    // Verify all required layers are available.
    for(u32 i = 0; i < required_validation_layer_count; ++i)
    {
        VEINFO("Searching for layer: %s...", required_validation_layer_names[i]);
        b8 found = FALSE;
        for(u32 j = 0; j < available_layer_count; ++j)
        {
            if(string_equal(required_validation_layer_names[i], available_layers[j].layerName))
            {
                found = TRUE;
                VEINFO("Found.");
                break;
            }
        }

        if(!found)
        {
            VEFATAL("Required validation layer is missing: %s", required_validation_layer_names[i]);
            return FALSE;
        }
    }

    VEINFO("All required validation layers are present.");
#endif

    create_info.enabledLayerCount = required_validation_layer_count;
    create_info.ppEnabledLayerNames = required_validation_layer_names;

    VK_CHECK(vkCreateInstance(&create_info, context.allocator, &context.instance));
    VEINFO("Vulkan Instance Created.");

    // Debugger.
#if defined(_DEBUG)
    VEDEBUG("Creating Vulkan debugger...");
    u32 log_severity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT; // |
                                                                        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT      
                                                                        // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT

    VkDebugUtilsMessengerCreateInfoEXT debug_create_info = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debug_create_info.messageSeverity = log_severity;
    debug_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.pfnUserCallback = vk_debug_callback;

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    VEASSERT_MSG(func, "Failed to create debug messenger!");
    VK_CHECK(func(context.instance, &debug_create_info, context.allocator, &context.debug_messenger));
    VEDEBUG("Vulkan debugger created.");
#endif                    

    // Surface Creation.
    VEDEBUG("Creating Vulkan surface...");
    if(!platform_create_vulkan_surface(plat_state, &context))
    {
        VEERROR("Failed to create platform surface!");
        return FALSE;
    }

    // Device creation.
    if(!vulkan_device_create(&context))
    {
        VEERROR("Failed to create device!");
        return FALSE;
    }

    VEINFO("Vulkan Renderer initalized succesfully.");
    return TRUE;
}

void vulkan_renderer_backend_shutdown(struct renderer_backend* backend)
{
    VEDEBUG("Destroying Vulkan device...");
    vulkan_device_destroy(&context);

    VEDEBUG("Destroying Vulkan surface...");
    if(context.surface)
    {
        vkDestroySurfaceKHR(context.instance, context.surface, context.allocator);
        context.surface = 0;
    }

    VEDEBUG("Destroying Vulkan debugger...");
    if(context.debug_messenger)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = 
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debug_messenger, context.allocator);
    }

    VEDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkan_renderer_backend_on_resized(struct renderer_backend* backend, u16 width, u16 height)
{

}

b8 vulkan_renderer_backend_begin_frame(struct renderer_backend* backend, f32 delta_time)
{
    return TRUE;
}

b8 vulkan_renderer_backend_end_frame(struct renderer_backend* backend, f32 delta_time)
{
    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
    VkDebugUtilsMessageTypeFlagsEXT message_types,
    const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
    void* user_data)
{
    switch(message_severity)
    {
        default:
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            VEERROR(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            VEWARN(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            VEINFO(callback_data->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            VETRACE(callback_data->pMessage);
            break;
    }

    return VK_FALSE;
}