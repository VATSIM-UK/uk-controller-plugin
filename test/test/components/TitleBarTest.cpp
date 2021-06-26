#include "pch/pch.h"
#include "components/TitleBar.h"
#include "mock/MockEuroscopeRadarScreenLoopbackInterface.h"
#include "mock/MockGraphicsInterface.h"

using UKControllerPlugin::Components::TitleBar;
using testing::_;
using testing::Ref;

namespace UKControllerPluginTest::Components {
    class TitleBarTest : public testing::Test
    {
        public:
            TitleBarTest()
            {
                brush = std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(1, 2, 3));
                pen = std::make_shared<Gdiplus::Pen>(Gdiplus::Color(1, 2, 3));
                titlebar = TitleBar::Create(
                    title,
                    Gdiplus::Rect{10, 20, 100, 100}
                );
            }

            std::wstring title = L"Title";
            std::shared_ptr<Gdiplus::Pen> pen;
            std::shared_ptr<Gdiplus::Brush> brush;
            std::shared_ptr<TitleBar> titlebar;
            testing::NiceMock<Windows::MockGraphicsInterface> mockGraphics;
            testing::NiceMock<Euroscope::MockEuroscopeRadarScreenLoopbackInterface> mockRadarScreen;
    };

    TEST_F(TitleBarTest, TestItDrawsBase)
    {
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(_, _, _, _))
            .Times(0);

        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }

    TEST_F(TitleBarTest, TestItDrawsBackground)
    {
        EXPECT_CALL(
            this->mockGraphics,
            FillRectRect(GdiRectEq(Gdiplus::Rect{10, 20, 100, 100}), Ref(*this->brush))
        )
            .Times(1);

        EXPECT_EQ(this->titlebar, this->titlebar->WithBackgroundBrush(this->brush));
        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }

    TEST_F(TitleBarTest, TestItDrawsText)
    {
        EXPECT_CALL(this->mockGraphics,
                    DrawStringRect(this->title, GdiRectEq(Gdiplus::Rect{10, 20, 100, 100}), Ref(*this->brush)))
            .Times(1);

        EXPECT_EQ(this->titlebar, this->titlebar->WithTextBrush(this->brush));
        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }

    TEST_F(TitleBarTest, TestItDrawsBorder)
    {
        EXPECT_CALL(
            this->mockGraphics,
            DrawRectRect(GdiRectEq(Gdiplus::Rect{10, 20, 100, 100}), Ref(*this->pen))
        )
            .Times(1);

        EXPECT_EQ(this->titlebar, this->titlebar->WithBorder(this->pen));
        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }

    TEST_F(TitleBarTest, TestItIsDraggable)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        ON_CALL(this->mockGraphics, GetTransform)
            .WillByDefault(testing::Return(transform));

        EXPECT_EQ(this->titlebar, this->titlebar->WithDrag(1));

        RECT expectedRect = {60, 120, 160, 220};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "titleBar", RectEq(expectedRect), true))
            .Times(1);

        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }

    TEST_F(TitleBarTest, TestItIsMovable)
    {
        auto transform = std::make_shared<Gdiplus::Matrix>();
        transform->Translate(50, 100);

        ON_CALL(this->mockGraphics, GetTransform)
            .WillByDefault(testing::Return(transform));

        EXPECT_EQ(this->titlebar, this->titlebar->WithDrag(1));

        this->titlebar->WithPosition(Gdiplus::Rect{20, 30, 110, 120});

        RECT expectedRect = {70, 130, 180, 250};
        EXPECT_CALL(this->mockRadarScreen, RegisterScreenObject(1, "titleBar", RectEq(expectedRect), true))
            .Times(1);

        this->titlebar->Draw(mockGraphics, mockRadarScreen);
    }
} // namespace UKControllerPluginTest::Components
