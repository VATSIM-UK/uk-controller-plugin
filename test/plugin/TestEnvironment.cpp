#include "helper/TestEnvironment.h"
#include "theme/ThemeManager.h"

using UKControllerPlugin::Theme::ThemeManager;

class CoreTestEnvironment : public TestEnvironment
{
    public:
    void TearDown() override
    {
        ThemeManager::UnsetInstance();
        TestEnvironment::TearDown();
    }
};

auto TestEnvironment::CreateEnvironment() -> TestEnvironment*
{
    return new CoreTestEnvironment;
}
