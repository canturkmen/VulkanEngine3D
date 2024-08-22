#include "memory/linear_allocator_tests.h"

#include <core/logger.h>

int main()
{
    // Always initialize the test manager first.
    test_manager_init();

    // TODO: Add test registrations here.
    linear_allocator_register_tests();

    VEDEBUG("Starting tests...");

    test_manager_run_tests();

    return 0;
}