#include "application.h"

#include "platform/platform.h"

#include "core/logger.h"
#include "core/vememory.h"
#include "core/event.h"

#include "game_types.h"

typedef struct application_state 
{
    game* game_inst;
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    f64 last_time;
} application_state;

static b8 initialized = FALSE;
static application_state app_state;

b8 application_create(game* game_inst)
{
    if(initialized)
    {
        VEERROR("application_create called more than once.");
        return FALSE;
    }

    app_state.game_inst = game_inst;

    // Initialize subsystems.
    initialize_logging();

    // TODO: Remove this.
    VEFATAL("A test message :%f", 3.14f);
    VEERROR("A test message :%f", 3.14f);
    VEWARN("A test message :%f", 3.14f);
    VEINFO("A test message :%f", 3.14f);
    VEDEBUG("A test message :%f", 3.14f);
    VETRACE("A test message: %f", 3.14f);

    app_state.is_running = TRUE;
    app_state.is_suspended = FALSE;

    if(!event_initialize())
    {
        VEERROR("Event system failed initialization. Application cannot continue.");
        return FALSE;
    }
    
    if(!platform_startup(&app_state.platform, 
        game_inst->app_config.name,
        game_inst->app_config.start_pos_x, 
        game_inst->app_config.start_pos_y, 
        game_inst->app_config.start_pos_width, 
        game_inst->app_config.start_pos_height)) 
    return FALSE;

    if(!app_state.game_inst->initialize(app_state.game_inst))
    {
        VEFATAL("Game failed to initalize!");
        return FALSE;
    }

    app_state.game_inst->on_resize(app_state.game_inst, app_state.width, app_state.height);

    initialized = TRUE;

    return TRUE;
}

b8 application_run()
{
    VEINFO(get_memory_usage_str());

    while(app_state.is_running)
    {
        if(!platform_pump_messages(&app_state.platform))
            app_state.is_running = FALSE;   

        if(!app_state.is_suspended)
        {
            if(!app_state.game_inst->update(app_state.game_inst, (f32)0)) 
            {
                VEFATAL("Game update failed, shutting down.");
                app_state.is_running = FALSE;
                break;
            }

            // Call the game's render routine.
            if(!app_state.game_inst->render(app_state.game_inst, (f32)0))
            {
                VEFATAL("Game render failed, shutting down");
                app_state.is_running = FALSE;
                break;
            }
        }
    }

    app_state.is_running = FALSE;
    event_shutdown();
    platform_shutdown(&app_state.platform);
    return TRUE;
}