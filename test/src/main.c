#include <core/logger.h>
#include <core/asserts.h>

// TODO: Test
#include <platform/platform.h>

int main(void)
{
    VEFATAL("A test message :%f", 3.14f);
    VEERROR("A test message :%f", 3.14f);
    VEWARN("A test message :%f", 3.14f);
    VEINFO("A test message :%f", 3.14f);
    VEDEBUG("A test message :%f", 3.14f);
    VETRACE("A test message: %f", 3.14f);

    platform_state state;
    if(platform_startup(&state, "Vulkan Engine 3D Testbed", 100, 100, 1280, 720)) {
        while(TRUE) {
            platform_pump_messages(&state);
        }
    }
    platform_shutdown(&state);

    return 0;
}
