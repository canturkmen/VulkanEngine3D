#pragma once

#include "renderer/renderer_backend.h"

b8 vulkan_renderer_backend_initialize(struct renderer_backend* backend, const char* application_name);
void vulkan_renderer_backend_shutdown(struct renderer_backend* backend);

void vulkan_renderer_backend_on_resized(struct renderer_backend* backend, u16 width, u16 height);

b8 vulkan_renderer_backend_begin_frame(struct renderer_backend* backend, f32 delta_time);
void vulkan_renderer_update_global_state(mat4 projection, mat4 view, vec3 view_position, vec4 ambient_color, i32 mode);
b8 vulkan_renderer_backend_end_frame(struct renderer_backend* backend, f32 delta_time);

void vulkan_backend_update_object( mat4 model);

