#include "wake/DepartureWakeInterval.h"
#include "wake/WakeCategory.h"

using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::WakeCategory;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryTest : public testing::Test
    {
        public:
        WakeCategoryTest()
            : wakeIntervals(
                  {std::make_shared<DepartureWakeInterval>(3, 60, "nm", true),
                   std::make_shared<DepartureWakeInterval>(3, 30, "nm", false)}),
              category(123, "LM", "Lower Medium", 20, wakeIntervals),
              subsequentCategory1(1, "LM", "Lower Medium", 20, {}), subsequentCategory2(3, "LM", "Lower Medium", 20, {})
        {
        }

        std::list<std::shared_ptr<DepartureWakeInterval>> wakeIntervals;
        WakeCategory category;
        WakeCategory subsequentCategory1;
        WakeCategory subsequentCategory2;
    };

    TEST_F(WakeCategoryTest, ItHasAnId)
    {
        EXPECT_EQ(123, category.Id());
    }

    TEST_F(WakeCategoryTest, ItHasACode)
    {
        EXPECT_EQ("LM", category.Code());
    }

    TEST_F(WakeCategoryTest, ItHasADescription)
    {
        EXPECT_EQ("Lower Medium", category.Description());
    }

    TEST_F(WakeCategoryTest, ItHasARelativeWeighting)
    {
        EXPECT_EQ(20, category.RelativeWeighting());
    }

    TEST_F(WakeCategoryTest, ItHasWakeIntervals)
    {
        EXPECT_EQ(wakeIntervals, category.SubsequentDepartureIntervals());
    }

    TEST_F(WakeCategoryTest, ItReturnsSubsequentWakeIntervalIntermediate)
    {
        EXPECT_EQ(wakeIntervals.front(), category.DepartureInterval(subsequentCategory2, true));
    }

    TEST_F(WakeCategoryTest, ItReturnsSubsequentWakeIntervalNotIntermediate)
    {
        EXPECT_EQ(wakeIntervals.back(), category.DepartureInterval(subsequentCategory2, false));
    }

    TEST_F(WakeCategoryTest, ItReturnsNullptrNoDepartureIntervalFound)
    {
        EXPECT_EQ(nullptr, category.DepartureInterval(subsequentCategory1, false));
    }
} // namespace UKControllerPluginTest::Wake
