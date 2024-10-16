#include "renderer_frontend.h"
#include "renderer_backend.h"

#include "core/logger.h"
#include "core/vememory.h"

#include "math/vemath.h"

// Backend render context.
static renderer_backend* backend = 0; 

b8 renderer_initialize(const char* application_name, struct platform_state* plat_state)
{
    backend = veallocate(sizeof(renderer_backend), MEMORY_TAG_RENDERER);
    
    // TODO: Make this configurable.
    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_number = 0;

    if(!backend->initialize(backend, application_name, plat_state))
    {
        VEFATAL("Renderer backend failed to initalize. Shutting down.");
        return false;
    } 

    return true;
}

void renderer_shutdown()
{
    backend->shutdown(backend);
    vefree(backend, sizeof(renderer_backend), MEMORY_TAG_RENDERER);
}   

b8 renderer_begin_frame(f32 delta_time)
{
    return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time)
{
    b8 result = backend->end_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

void renderer_on_resized(u16 width, u16 height)
{
    if(backend)
        backend->resized(backend, width, height);
    else
        VEWARN("Renderer backend does not exist to accept resize: %i,  %i", width, height);
}

b8 renderer_draw_frame(render_packet* packet)
{   
    // If the begin frame returned succesfully, mid-frame operations may continue.
    if(renderer_begin_frame(packet->delta_time))
    {
        mat4 projection = mat4_perspective(deg_to_rad(45.0f), 1280/720.0f, 0.1f, 1000.0f);
        static f32 z = -1.0f;
        z -= 0.01f;
        mat4 view = mat4_translation((vec3){0, 0, z});

        backend->update_global_state(projection, view, vec3_zero(), vec4_one(), 0);

        // End the frame.
        b8 result = renderer_end_frame(packet->delta_time);
        if(!result)
        {
            VEERROR("renderer_end_frame failed. Application shuttting down...");
            return false;
        }
    }

    return true;
}   