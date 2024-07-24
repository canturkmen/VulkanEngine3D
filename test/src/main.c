#include <core/logger.h>
#include <core/asserts.h>

#include "core/application.h"

int main(void)
{
    application_config config = {
        100,
        100,
        1280,
        720,
        "Vulkan Engine 3D Test"
    };

    application_create(&config);
    application_run();

    return 0;
}