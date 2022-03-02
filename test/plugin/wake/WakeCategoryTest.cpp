#include "wake/ArrivalWakeInterval.h"
#include "wake/DepartureWakeInterval.h"
#include "wake/WakeCategory.h"

using UKControllerPlugin::Wake::ArrivalWakeInterval;
using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::WakeCategory;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryTest : public testing::Test
    {
        public:
        WakeCategoryTest()
            : departureWakeIntervals(
                  {std::make_shared<DepartureWakeInterval>(3, 60, "nm", true),
                   std::make_shared<DepartureWakeInterval>(3, 30, "nm", false)}),
              arrivalWakeIntervals({
                  std::make_shared<ArrivalWakeInterval>(3, 3.5),
                  std::make_shared<ArrivalWakeInterval>(6, 4.5),
              }),
              category(123, "LM", "Lower Medium", 20, departureWakeIntervals, arrivalWakeIntervals),
              subsequentCategory1(1, "LM", "Lower Medium", 20, {}, {}),
              subsequentCategory2(3, "LM", "Lower Medium", 20, {}, {})
        {
        }

        std::list<std::shared_ptr<DepartureWakeInterval>> departureWakeIntervals;
        std::list<std::shared_ptr<ArrivalWakeInterval>> arrivalWakeIntervals;
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
        EXPECT_EQ(departureWakeIntervals, category.SubsequentDepartureIntervals());
    }

    TEST_F(WakeCategoryTest, ItReturnsSubsequentWakeIntervalIntermediate)
    {
        EXPECT_EQ(departureWakeIntervals.front(), category.DepartureInterval(subsequentCategory2, true));
    }

    TEST_F(WakeCategoryTest, ItReturnsSubsequentWakeIntervalNotIntermediate)
    {
        EXPECT_EQ(departureWakeIntervals.back(), category.DepartureInterval(subsequentCategory2, false));
    }

    TEST_F(WakeCategoryTest, ItReturnsNullptrNoDepartureIntervalFound)
    {
        EXPECT_EQ(nullptr, category.DepartureInterval(subsequentCategory1, false));
    }

    TEST_F(WakeCategoryTest, ItHasArrivalWakeIntervals)
    {
        EXPECT_EQ(arrivalWakeIntervals, category.SubsequentArrivalIntervals());
    }

    TEST_F(WakeCategoryTest, ItReturnsSubsequentArrivalWakeInterval)
    {
        EXPECT_EQ(arrivalWakeIntervals.front(), category.ArrivalInterval(subsequentCategory2));
    }

    TEST_F(WakeCategoryTest, ItReturnsNullptrNoArrivalWakeInterval)
    {
        EXPECT_EQ(nullptr, category.ArrivalInterval(subsequentCategory1));
    }
} // namespace UKControllerPluginTest::Wake
