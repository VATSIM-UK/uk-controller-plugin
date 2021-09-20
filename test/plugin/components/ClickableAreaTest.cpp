#include "components/ClickableArea.h"

using UKControllerPlugin::Components::ClickableArea;

namespace UKControllerPluginTest::Components {
    class ClickableAreaTest : public testing::Test
    {
        public:
        testing::NiceMock<Windows::MockGraphicsInterface> mockGraphics;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
    };

    TEST_F(ClickableAreaTest, TestItApplys)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        auto area = ClickableArea::Create(Gdiplus::Rect{10, 20, 100, 100}, 1, "test", true);

        ON_CALL(this->mockGraphics, GetTransform).WillByDefault(testing::Return(transform));

        RECT expectedRect = {60, 120, 160, 220};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "test", RectEq(expectedRect), true)).Times(1);

        area->Apply(this->mockGraphics, this->mockRadarScreen);
    }

    TEST_F(ClickableAreaTest, TestItCanBeMoved)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        auto area = ClickableArea::Create(Gdiplus::Rect{50, 90, 55, 55}, 1, "test", false);
        area->WithPosition(Gdiplus::Rect{10, 20, 100, 100});

        ON_CALL(this->mockGraphics, GetTransform).WillByDefault(testing::Return(transform));

        RECT expectedRect = {60, 120, 160, 220};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "test", RectEq(expectedRect), false)).Times(1);

        area->Apply(this->mockGraphics, this->mockRadarScreen);
    }
} // namespace UKControllerPluginTest::Components
