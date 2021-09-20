#include "components/Button.h"

using testing::_;
using testing::Ref;
using UKControllerPlugin::Components::Button;

namespace UKControllerPluginTest::Components {
    class ButtonTest : public testing::Test
    {
        public:
        ButtonTest()
        {
            brush = std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(1, 2, 3));
            pen = std::make_shared<Gdiplus::Pen>(Gdiplus::Color(1, 2, 3));
            button = Button::Create(
                Gdiplus::Rect{10, 20, 100, 100},
                1,
                "description",
                [this](UKControllerPlugin::Windows::GdiGraphicsInterface& graphics, Gdiplus::Rect area) {
                    this->drawFunctionCalled = true;
                    EXPECT_TRUE(this->expectedDrawArea.Equals(area));
                });
        }

        bool drawFunctionCalled = false;
        Gdiplus::Rect expectedDrawArea;
        std::shared_ptr<Gdiplus::Pen> pen;
        std::shared_ptr<Gdiplus::Brush> brush;
        std::shared_ptr<Button> button;
        testing::NiceMock<Windows::MockGraphicsInterface> mockGraphics;
        testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
    };

    TEST_F(ButtonTest, TestItDraws)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        ON_CALL(this->mockGraphics, GetTransform).WillByDefault(testing::Return(transform));

        RECT expectedRect = {60, 120, 160, 220};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "description", RectEq(expectedRect), false))
            .Times(1);

        this->expectedDrawArea = Gdiplus::Rect{10, 20, 100, 100};

        this->button->Draw(mockGraphics, mockRadarScreen);
        EXPECT_TRUE(this->drawFunctionCalled);
    }

    TEST_F(ButtonTest, TestItCanBeMoved)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        ON_CALL(this->mockGraphics, GetTransform).WillByDefault(testing::Return(transform));

        this->button->WithPosition(Gdiplus::Rect{20, 30, 110, 120});
        this->expectedDrawArea = Gdiplus::Rect{20, 30, 110, 120};

        RECT expectedRect = {70, 130, 180, 250};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "description", RectEq(expectedRect), false))
            .Times(1);

        this->button->Draw(mockGraphics, mockRadarScreen);
        EXPECT_TRUE(this->drawFunctionCalled);
    }
} // namespace UKControllerPluginTest::Components
