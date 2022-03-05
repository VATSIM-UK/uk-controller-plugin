#include "hold/ProximityHold.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Hold {
    class ProximityHoldTest : public testing::Test
    {
        public:
        ProximityHoldTest()
            : entryTime(std::chrono::system_clock::now()), hold("BAW123", "TIMBA"),
              holdEntered("BAW456", "WILLO", entryTime)
        {
            SetTestNow(std::chrono::system_clock::now());
        }

        std::chrono::system_clock::time_point entryTime;
        ProximityHold hold;
        ProximityHold holdEntered;
    };

    TEST_F(ProximityHoldTest, ItHasACallsign)
    {
        EXPECT_EQ("BAW123", hold.Callsign());
    }

    TEST_F(ProximityHoldTest, ItHasANavaid)
    {
        EXPECT_EQ("TIMBA", hold.Navaid());
    }

    TEST_F(ProximityHoldTest, ItStartsNotEntered)
    {
        EXPECT_FALSE(hold.HasEntered());
    }

    TEST_F(ProximityHoldTest, ItCanBeEntered)
    {
        hold.Enter();
        EXPECT_TRUE(hold.HasEntered());
    }

    TEST_F(ProximityHoldTest, EnteringSetsTheEnterTime)
    {
        EXPECT_EQ((std::chrono::system_clock::time_point::min)(), hold.EnteredAt());
        hold.Enter();
        EXPECT_EQ(TimeNow(), hold.EnteredAt());
    }

    TEST_F(ProximityHoldTest, ItStartsEnteredIfTimeProvided)
    {
        EXPECT_TRUE(holdEntered.HasEntered());
        EXPECT_EQ(entryTime, holdEntered.EnteredAt());
    }
} // namespace UKControllerPluginTest::Hold
