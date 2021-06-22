#include "pch/pch.h"
#include "components/BrushSwitcher.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Components::BrushSwitcher;

namespace UKControllerPluginTest::Components {
    class BrushSwitcherTest : public testing::Test
    {
        public:
            BrushSwitcherTest()
            {
                UKControllerPlugin::Time::SetTestNow(std::chrono::system_clock::now());
                baseBrush = std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(0, 0, 0));
                extraBrush1 = std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(1, 1, 1));
                extraBrush2 = std::make_shared<Gdiplus::SolidBrush>(Gdiplus::Color(2, 2, 2));
                switcher = BrushSwitcher::Create(baseBrush, std::chrono::seconds(2));
                switcher->AdditionalBrush(extraBrush1);
                switcher->AdditionalBrush(extraBrush2);
            }

            std::shared_ptr<Gdiplus::Brush> baseBrush;
            std::shared_ptr<Gdiplus::Brush> extraBrush1;
            std::shared_ptr<Gdiplus::Brush> extraBrush2;
            std::shared_ptr<BrushSwitcher> switcher;
    };

    TEST_F(BrushSwitcherTest, BaseReturnsBaseBrush)
    {
        EXPECT_EQ(baseBrush, switcher->Base());
    }

    TEST_F(BrushSwitcherTest, NextStartsWithFirstBrush)
    {
        EXPECT_EQ(extraBrush1, switcher->Next());
    }

    TEST_F(BrushSwitcherTest, NextReturnsCurrentBrushIfPhaseNotPassed)
    {
        EXPECT_EQ(extraBrush1, switcher->Next());
        EXPECT_EQ(extraBrush1, switcher->Next());
    }

    TEST_F(BrushSwitcherTest, NextReturnsSubsequentColoursAsPhasesPass)
    {
        EXPECT_EQ(extraBrush1, switcher->Next());
        UKControllerPlugin::Time::SetTestNow(UKControllerPlugin::Time::TimeNow() + std::chrono::seconds(3));
        EXPECT_EQ(extraBrush2, switcher->Next());
        UKControllerPlugin::Time::SetTestNow(UKControllerPlugin::Time::TimeNow() + std::chrono::seconds(3));
        EXPECT_EQ(baseBrush, switcher->Next());
        UKControllerPlugin::Time::SetTestNow(UKControllerPlugin::Time::TimeNow() + std::chrono::seconds(3));
        EXPECT_EQ(extraBrush1, switcher->Next());
    }
} // namespace UKControllerPluginTest::Components
