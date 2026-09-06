#include "components/StandardButtons.h"
#include "graphics/GdiplusBrushes.h"

using testing::_;
using UKControllerPlugin::Components::CloseButton;
using UKControllerPlugin::Components::CollapseButton;
using UKControllerPlugin::Windows::GdiplusBrushes;

namespace UKControllerPluginTest::Components {

    class StandardButtonsTest : public testing::Test
    {
        public:
        StandardButtonsTest() = default;
        GdiplusBrushes brushes;
        testing::NiceMock<Windows::MockGraphicsInterface> mockGraphics;
    };

    TEST_F(StandardButtonsTest, CloseButtonThemedDoesNotCrash)
    {
        auto button = CloseButton(brushes);
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CollapseButtonThemedDoesNotCrash)
    {
        auto button = CollapseButton(brushes, []() { return false; });
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CloseButtonWithColorDoesNotCrash)
    {
        auto button = CloseButton(Gdiplus::Color(255, 0, 0));
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CollapseButtonWithColorDoesNotCrash)
    {
        auto button = CollapseButton(Gdiplus::Color(255, 0, 0), []() { return false; });
        EXPECT_NE(nullptr, button.target_type().name());
    }
} // namespace UKControllerPluginTest::Components
