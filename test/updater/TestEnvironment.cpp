#include "helper/TestEnvironment.h"

auto TestEnvironment::CreateEnvironment() -> TestEnvironment*
{
    return new TestEnvironment;
}
