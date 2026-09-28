#include "components/StandardButtons.h"

using testing::_;
using UKControllerPlugin::Components::CloseButton;
using UKControllerPlugin::Components::CollapseButton;
using UKControllerPlugin::Theme::PaletteKey;

namespace UKControllerPluginTest::Components {

    class StandardButtonsTest : public testing::Test
    {
        public:
        StandardButtonsTest() = default;
        testing::NiceMock<Windows::MockGraphicsInterface> mockGraphics;
    };

    TEST_F(StandardButtonsTest, CloseButtonThemedDoesNotCrash)
    {
        auto button = CloseButton();
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CollapseButtonThemedDoesNotCrash)
    {
        auto button = CollapseButton([]() { return false; });
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CloseButtonWithPaletteKeyDoesNotCrash)
    {
        auto button = CloseButton(PaletteKey::Background);
        EXPECT_NE(nullptr, button.target_type().name());
    }

    TEST_F(StandardButtonsTest, CollapseButtonWithPaletteKeyDoesNotCrash)
    {
        auto button = CollapseButton(PaletteKey::Background, []() { return false; });
        EXPECT_NE(nullptr, button.target_type().name());
    }
} // namespace UKControllerPluginTest::Components
