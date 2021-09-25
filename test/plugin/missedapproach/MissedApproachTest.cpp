#include "missedapproach/MissedApproach.h"
#include "time/SystemClock.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::Time::SetTestNow;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachTest : public testing::Test
    {
        public:
        MissedApproachTest()
            : expiresAt(std::chrono::system_clock::now() + std::chrono::minutes(5)), missed("BAW123", expiresAt)
        {
            SetTestNow(std::chrono::system_clock::now());
        }

        std::chrono::system_clock::time_point expiresAt;
        class MissedApproach missed;
    };

    TEST_F(MissedApproachTest, ItHasACallsign)
    {
        EXPECT_EQ("BAW123", missed.Callsign());
    }

    TEST_F(MissedApproachTest, ItHasAnExpiryTime)
    {
        EXPECT_EQ(expiresAt, missed.ExpiresAt());
    }

    TEST_F(MissedApproachTest, ItsNotExpiredIfExpiryTimeHasntPassed)
    {
        EXPECT_FALSE(missed.IsExpired());
    }

    TEST_F(MissedApproachTest, ItsExpiredIfExpiryTimeHasPassed)
    {
        class MissedApproach missed2("BAW123", std::chrono::system_clock::now() - std::chrono::seconds(5));
        EXPECT_TRUE(missed2.IsExpired());
    }
} // namespace UKControllerPluginTest::MissedApproach
