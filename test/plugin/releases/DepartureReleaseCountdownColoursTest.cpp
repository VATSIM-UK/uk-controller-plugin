#include "releases/DepartureReleaseColours.h"
#include "releases/DepartureReleaseCountdownColours.h"
#include "time/SystemClock.h"

using testing::Test;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Releases {

    class DepartureReleaseCountdownColoursTest : public Test
    {
        public:
            void SetUp() override
            {
                SetTestNow(std::chrono::system_clock::now());
            }
    };

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsPlentyOfTimeForLargeTimeLeft)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerPlentyOfTime,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(100))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsPlentyOfTimeJustAboveBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerPlentyOfTime,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(31))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCloseOnBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerClose,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(30))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCloseBelowBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerClose,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(29))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsClose)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerClose,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(15))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCloseAboveLowerBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerClose,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(11))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCriticalAtLowerBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerExpired,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(10))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCriticalBelowLowerBoundary)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerExpired,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() + std::chrono::seconds(3))
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCriticalAtZero)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerExpired,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow())
        );
    }

    TEST_F(DepartureReleaseCountdownColoursTest, ItReturnsCriticalWhenExpired)
    {
        EXPECT_EQ(
            UKControllerPlugin::Releases::releaseTimerExpired,
            UKControllerPlugin::Releases::TimeUntilExpiryColour(TimeNow() - + std::chrono::seconds(3))
        );
    }
} // namespace UKControllerPluginTest::Releases
