#include "game.h"

#include <core/logger.h>
#include <core/input.h>
#include <core/vememory.h>

b8 game_initalize(game* game_inst)
{
    return true;
}

b8 game_update(game* game_inst, f32 delta_time)
{
    static u64 alloc_count = 0;
    u64 prev_alloc_count = alloc_count;
    alloc_count = get_memory_alloc_count();
    if(input_is_key_up('M') && input_was_key_down('M'))
        VEDEBUG("Allocations: %llu (%llu this frame)", alloc_count, alloc_count - prev_alloc_count);

    return true;
}

b8 game_render(game* game_inst, f32 delta_time)
{
    return true;
}

void game_on_resize(game* game_inst, u32 width, u32 height)
{
    
}  