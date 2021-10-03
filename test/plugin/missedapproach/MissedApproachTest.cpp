#include "missedapproach/MissedApproach.h"
#include "time/SystemClock.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachTest : public testing::Test
    {
        public:
        MissedApproachTest() : expiresAt(GetExpiresAt()), missed(1, "BAW123", expiresAt)
        {
        }

        [[nodiscard]] static auto GetExpiresAt() -> std::chrono::system_clock::time_point
        {
            SetTestNow(std::chrono::system_clock::now());
            return TimeNow() + std::chrono::minutes(5);
        }

        std::chrono::system_clock::time_point expiresAt;
        class MissedApproach missed;
    };

    TEST_F(MissedApproachTest, ItHasAnId)
    {
        EXPECT_EQ(1, missed.Id());
    }

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
        class MissedApproach missed2(2, "BAW123", std::chrono::system_clock::now() - std::chrono::seconds(5));
        EXPECT_TRUE(missed2.IsExpired());
    }

    TEST_F(MissedApproachTest, ItHasACreatedAtTimeWhichIsSetToCurrentTime)
    {
        EXPECT_EQ(TimeNow(), missed.CreatedAt());
    }
} // namespace UKControllerPluginTest::MissedApproach
