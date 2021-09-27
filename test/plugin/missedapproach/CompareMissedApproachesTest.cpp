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
            missed1 = std::make_shared<class MissedApproach>("BAW123", TimeNow());
            missed2 = std::make_shared<class MissedApproach>("BAW456", TimeNow());
        }

        CompareMissedApproaches compare;
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
    };

    TEST_F(CompareMissedApproachesTest, LessThanStringReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare(missed1, "BAW456"));
    }

    TEST_F(CompareMissedApproachesTest, LessThanStringReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare(missed2, "BAW123"));
    }

    TEST_F(CompareMissedApproachesTest, LessThanMessageReturnsTrueIfLessThan)
    {
        EXPECT_TRUE(compare("BAW123", missed2));
    }

    TEST_F(CompareMissedApproachesTest, LessThanMessageReturnsFalseIfNotLessThan)
    {
        EXPECT_FALSE(compare("BAW999", missed1));
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
