#include <core/logger.h>
#include <core/asserts.h>

int main(void)
{
    VEFATAL("A test message :%f", 3.14f);
    VEERROR("A test message :%f", 3.14f);
    VEWARN("A test message :%f", 3.14f);
    VEINFO("A test message :%f", 3.14f);
    VEDEBUG("A test message :%f", 3.14f);
    VETRACE("A test message: %f", 3.14f);

    VEASSERT_MSG(1 == 0, "Please fix it");

    return 0;
}
