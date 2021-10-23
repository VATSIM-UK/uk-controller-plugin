#include "missedapproach/CompareMissedApproaches.h"
#include "missedapproach/MissedApproach.h"
#include "time/SystemClock.h"

using UKControllerPlugin::MissedApproach::CompareMissedApproaches;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::MissedApproach {
    class CompareMissedApproachesTest : public ::testing::Test
    {
        public:
        CompareMissedApproachesTest()
        {
            missed1 = std::make_shared<class MissedApproach>(3, "BAW123", TimeNow(), true);
            missed2 = std::make_shared<class MissedApproach>(4, "BAW456", TimeNow(), true);
        }

        CompareMissedApproaches compare;
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
    };

    TEST_F(CompareMissedApproachesTest, LessThanIntReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(missed1, 4));
    }

    TEST_F(CompareMissedApproachesTest, LessThanIntReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(missed2, 3));
    }

    TEST_F(CompareMissedApproachesTest, LessThanMessageReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(3, missed2));
    }

    TEST_F(CompareMissedApproachesTest, LessThanMessageReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(4, missed1));
    }

    TEST_F(CompareMissedApproachesTest, LessThanCompareMessagesReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(missed1, missed2));
    }

    TEST_F(CompareMissedApproachesTest, LessThanCompareMessagesReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(missed2, missed1));
    }
} // namespace UKControllerPluginTest::MissedApproach
