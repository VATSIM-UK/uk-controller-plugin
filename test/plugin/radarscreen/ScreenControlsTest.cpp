#include "graphics/GdiplusBrushes.h"
#include "helper/Matchers.h"
#include "helper/TestingFunctions.h"
#include "mock/MockConfigurableDisplay.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "plugin/PopupMenuItem.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "radarscreen/ScreenControls.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::Plugin::PopupMenuItem;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface;
using UKControllerPlugin::RadarScreen::ScreenControls;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;

namespace UKControllerPluginTest::RadarScreen {

    TEST(ScreenControls, LeftClickDoesNothingIfWrongIdPassed)
    {
        GdiplusBrushes brushes;
        ConfigurableDisplayCollection configurables;
        ScreenControls controls(1, configurables, brushes);
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

        controls.LeftClick(mockRadar, 2, "", {}, {});
    }

    TEST(ScreenControls, LeftClickOpensAPopup)
    {
        GdiplusBrushes brushes;
        ConfigurableDisplayCollection configurables;
        std::shared_ptr<ConfigurableDisplayInterface> display1 =
            std::make_shared<MockConfigurableDisplay>("firstValue1", "secondValue1");
        std::shared_ptr<ConfigurableDisplayInterface> display2 =
            std::make_shared<MockConfigurableDisplay>("firstValue2", "secondValue2");
        configurables.RegisterDisplay(display1);
        configurables.RegisterDisplay(display2);

        ScreenControls controls(1, configurables, brushes);
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

        RECT radarArea = {0, 0, 1024, 768};
        RECT menuRect = {radarArea.right - 25, radarArea.bottom - 25, radarArea.right, radarArea.bottom};

        EXPECT_CALL(mockRadar, GetRadarViewport()).Times(1).WillOnce(Return(radarArea));

        EXPECT_CALL(mockRadar, ToogleMenu(RectEq(menuRect), "UKCP Options", 1)).Times(1);

        EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display1->GetConfigurationMenuItem()))).Times(1);

        EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display2->GetConfigurationMenuItem()))).Times(1);

        controls.LeftClick(mockRadar, 1, "", {}, {});
    }

    TEST(ScreenControls, RightClickDoesNothingIfWrongIdPassed)
    {
        GdiplusBrushes brushes;
        ConfigurableDisplayCollection configurables;
        ScreenControls controls(1, configurables, brushes);
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

        controls.RightClick(2, "", mockRadar);
    }

    TEST(ScreenControls, RightClickOpensAPopup)
    {
        GdiplusBrushes brushes;
        ConfigurableDisplayCollection configurables;
        std::shared_ptr<ConfigurableDisplayInterface> display1 =
            std::make_shared<MockConfigurableDisplay>("firstValue1", "secondValue1");
        std::shared_ptr<ConfigurableDisplayInterface> display2 =
            std::make_shared<MockConfigurableDisplay>("firstValue2", "secondValue2");
        configurables.RegisterDisplay(display1);
        configurables.RegisterDisplay(display2);

        ScreenControls controls(1, configurables, brushes);
        StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

        RECT radarArea = {0, 0, 1024, 768};
        RECT menuRect = {radarArea.right - 25, radarArea.bottom - 25, radarArea.right, radarArea.bottom};

        EXPECT_CALL(mockRadar, GetRadarViewport()).Times(1).WillOnce(Return(radarArea));

        EXPECT_CALL(mockRadar, ToogleMenu(RectEq(menuRect), "UKCP Options", 1)).Times(1);

        EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display1->GetConfigurationMenuItem()))).Times(1);

        EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display2->GetConfigurationMenuItem()))).Times(1);

        controls.RightClick(1, "", mockRadar);
    }
} // namespace UKControllerPluginTest::RadarScreen
