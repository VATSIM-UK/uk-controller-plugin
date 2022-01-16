#include "wake/DepartureWakeInterval.h"
#include "wake/WakeCategory.h"

using UKControllerPlugin::Wake::DepartureWakeInterval;
using UKControllerPlugin::Wake::WakeCategory;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryTest : public testing::Test
    {
        public:
        WakeCategoryTest()
            : wakeIntervals({std::make_shared<DepartureWakeInterval>(3, 60, "nm", true)}),
              category(123, "LM", "Lower Medium", 20, wakeIntervals)
        {
        }

        std::list<std::shared_ptr<DepartureWakeInterval>> wakeIntervals;
        WakeCategory category;
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
} // namespace UKControllerPluginTest::Wake
