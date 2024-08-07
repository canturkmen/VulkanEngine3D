#include "vulkan_swapchain.h"

#include "vulkan_device.h"

#include "core/logger.h"
#include "core/vememory.h"

void create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* swapchain);   
void destroy(vulkan_context* context, vulkan_swapchain* swapchain);

void vulkan_swapchain_create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain)
{
    // Simply create a new one.
    create(context, width, height, out_swapchain);
}

void vulkan_swapchain_recreate(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* out_swapchain)
{
    // Destroy the old and create a new one.
    destroy(context, out_swapchain);
    create(context, width, height, out_swapchain);
}

void vulkan_swapchain_destroy(vulkan_context* context, vulkan_swapchain* swapchain)
{
    destroy(context, swapchain);
}

b8 vulkan_swapchain_acquire_next_image_index(
    vulkan_context* context, 
    vulkan_swapchain* swapchain,
    u64 timeout_ns,
    VkSemaphore image_available_semaphore, 
    VkFence fence, 
    u32* out_image_index)
{
    VkResult result = vkAcquireNextImageKHR(context->device.logical_device, 
    swapchain->handle, timeout_ns, image_available_semaphore, fence, out_image_index);

    if(result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        // Trigger swapchain recreation, then boot out of the render loop.
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
        return FALSE;
    } else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        VEFATAL("Failed to acquire swapchain image!");
        return FALSE;
    }

    return TRUE;
}

void vulkan_swapchain_present(
    vulkan_context* context, 
    vulkan_swapchain* swapchain, 
    VkQueue graphics_queue, 
    VkQueue present_queue, 
    VkSemaphore render_complete_semaphore, 
    u32 present_image_index)
{
    // Return the image to the swapchain for presentation.
    VkPresentInfoKHR present_info = {VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &render_complete_semaphore;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &swapchain->handle;
    present_info.pImageIndices = &present_image_index;
    present_info.pResults = 0;

    VkResult result = vkQueuePresentKHR(present_queue, &present_info);
    if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        // Swapchain is out of date, suboptimal or a framebuffer resize has occured. Trigger swapchain recreation.
        vulkan_swapchain_recreate(context, context->framebuffer_width, context->framebuffer_height, swapchain);
    } else if(result != VK_SUCCESS)
        VEFATAL("Failed to acquire swapchain image!");
}

void create(vulkan_context* context, u32 width, u32 height, vulkan_swapchain* swapchain)
{
    VkExtent2D swapchain_extent = {width, height};
    swapchain->max_frames_in_flight = 2;

    // Choose a swap surface format.
    b8 found = FALSE;
    for(u32 i = 0; i < context->device.swapchain_support.format_count; ++i)
    {
        VkSurfaceFormatKHR format = context->device.swapchain_support.formats[i];
        // Preffered formats.
        if(format.format == VK_FORMAT_B8G8R8A8_UNORM 
        && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            swapchain->image_format = format;
            found = TRUE;
            break;
        }
    }

    if(!found)
        swapchain->image_format = context->device.swapchain_support.formats[0];

    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (u32 i = 0; i < context->device.swapchain_support.present_mode_count; ++i)
    {
        VkPresentModeKHR mode = context->device.swapchain_support.present_modes[i];
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            present_mode = mode;
            break;
        }
    }

    // Requery swapchain support.
    vulkan_device_query_swapchain_support(
        context->device.physical_device,
        context->surface,
        &context->device.swapchain_support
    );

    // Swapchain extent.
    if(context->device.swapchain_support.capabilities.currentExtent.width != UINT32_MAX)
        swapchain_extent = context->device.swapchain_support.capabilities.currentExtent;

    // Clamp the value allowed by the GPU.
    VkExtent2D min = context->device.swapchain_support.capabilities.minImageExtent;
    VkExtent2D max = context->device.swapchain_support.capabilities.maxImageExtent;
    swapchain_extent.width = VECLAMP(swapchain_extent.width, min.width, max.width);
    swapchain_extent.height = VECLAMP(swapchain_extent.height, min.height, max.height);

    u32 image_count = context->device.swapchain_support.capabilities.minImageCount + 1;
    if(context->device.swapchain_support.capabilities.maxImageCount > 0 
    && image_count > context->device.swapchain_support.capabilities.maxImageCount)
        image_count = context->device.swapchain_support.capabilities.maxImageCount;

    // Swapchain create info.
    VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    swapchain_create_info.surface = context->surface;
    swapchain_create_info.minImageCount = image_count;
    swapchain_create_info.imageFormat = swapchain->image_format.format;
    swapchain_create_info.imageColorSpace = swapchain->image_format.colorSpace;
    swapchain_create_info.imageExtent = swapchain_extent;
    swapchain_create_info.imageArrayLayers = 1;
    swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Setup the queue family indices.
    if(context->device.graphics_queue_index != context->device.present_queue_index)
    {
        u32 queue_family_indices[] = {(u32)context->device.graphics_queue_index, (u32)context->device.present_queue_index};
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_create_info.queueFamilyIndexCount = 2;
        swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
    }
    else
    {
        swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchain_create_info.queueFamilyIndexCount = 0;
        swapchain_create_info.pQueueFamilyIndices = 0;
    }
}

void destroy(vulkan_context* context, vulkan_swapchain* swapchain)
{

}
