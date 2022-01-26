#include "radarscreen/MenuToggleableDisplayInterface.h"

using UKControllerPlugin::RadarScreen::MenuToggleableDisplayInterface;

namespace UKControllerPluginTest::RadarScreen {
    class MockMenuToggleableDisplay :  public MenuToggleableDisplayInterface
    {
        public:
            MOCK_METHOD(std::string, MenuItem, (), (const, override));
            MOCK_METHOD(bool, IsVisible, (), (const, override));
            MOCK_METHOD(void, Toggle, (), (override));
    };
} // namespace UKControllerPluginTest::RadarScreen
