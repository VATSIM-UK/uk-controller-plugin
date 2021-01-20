#include "pch/pch.h"
#include "radarscreen/ScreenControls.h"
#include "radarscreen/ConfigurableDisplayCollection.h"
#include "graphics/GdiplusBrushes.h"
#include "helper/TestingFunctions.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "radarscreen/ConfigurableDisplayInterface.h"
#include "plugin/PopupMenuItem.h"
#include "helper/Matchers.h"
#include "mock/MockConfigurableDisplay.h"

using UKControllerPlugin::RadarScreen::ScreenControls;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayCollection;
using UKControllerPlugin::Windows::GdiplusBrushes;
using UKControllerPluginTest::Euroscope::MockEuroscopeRadarScreenLoopbackInterface;
using UKControllerPlugin::RadarScreen::ConfigurableDisplayInterface;
using UKControllerPlugin::Plugin::PopupMenuItem;
using ::testing::StrictMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace RadarScreen {

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
            std::shared_ptr<ConfigurableDisplayInterface> display1 = std::make_shared<MockConfigurableDisplay>(
                "firstValue1",
                "secondValue1"
            );
            std::shared_ptr<ConfigurableDisplayInterface> display2 = std::make_shared<MockConfigurableDisplay>(
                "firstValue2",
                "secondValue2"
            );
            configurables.RegisterDisplay(display1);
            configurables.RegisterDisplay(display2);

            ScreenControls controls(1, configurables, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

            RECT radarArea = { 0, 0, 1024, 768 };
            RECT menuRect = {
                radarArea.right - controls.controlWidth,
                radarArea.bottom - controls.controlHeight,
                radarArea.right,
                radarArea.bottom
            };

            EXPECT_CALL(mockRadar, GetRadarViewport())
                .Times(1)
                .WillOnce(Return(radarArea));

            EXPECT_CALL(mockRadar, ToogleMenu(RectEq(menuRect), controls.menuName, 1))
                .Times(1);

            EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display1->GetConfigurationMenuItem())))
                .Times(1);

            EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display2->GetConfigurationMenuItem())))
                .Times(1);

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
            std::shared_ptr<ConfigurableDisplayInterface> display1 = std::make_shared<MockConfigurableDisplay>(
                "firstValue1",
                "secondValue1"
            );
            std::shared_ptr<ConfigurableDisplayInterface> display2 = std::make_shared<MockConfigurableDisplay>(
                "firstValue2",
                "secondValue2"
            );
            configurables.RegisterDisplay(display1);
            configurables.RegisterDisplay(display2);

            ScreenControls controls(1, configurables, brushes);
            StrictMock<MockEuroscopeRadarScreenLoopbackInterface> mockRadar;

            RECT radarArea = { 0, 0, 1024, 768 };
            RECT menuRect = {
                radarArea.right - controls.controlWidth,
                radarArea.bottom - controls.controlHeight,
                radarArea.right,
                radarArea.bottom
            };

            EXPECT_CALL(mockRadar, GetRadarViewport())
                .Times(1)
                .WillOnce(Return(radarArea));

            EXPECT_CALL(mockRadar, ToogleMenu(RectEq(menuRect), controls.menuName, 1))
                .Times(1);

            EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display1->GetConfigurationMenuItem())))
                .Times(1);

            EXPECT_CALL(mockRadar, AddMenuItem(PopupMenuItemEq(display2->GetConfigurationMenuItem())))
                .Times(1);

            controls.RightClick(1, "", mockRadar);
        }
    }  // namespace RadarScreen
}  // namespace UKControllerPluginTest
