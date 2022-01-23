#include "sid/StandardInstrumentDeparture.h"

using ::testing::Test;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Sid {

    class StandardInstrumentDepartureTest : public Test
    {
        public:
        StandardInstrumentDepartureTest()
            : sid(1, 2, "WOTAN1Z", 6000, 55, 3, {1, 2}), sidWithDefaults(2, 2, "WOTAN1Z", 6000, 55, 3)
        {
        }

        StandardInstrumentDeparture sid;
        StandardInstrumentDeparture sidWithDefaults;
    };

    TEST_F(StandardInstrumentDepartureTest, ItHasANId)
    {
        EXPECT_EQ(1, sid.Id());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasARunwayId)
    {
        EXPECT_EQ(2, sid.RunwayId());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("WOTAN1Z", sid.Identifier());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasAnInitialAltitude)
    {
        EXPECT_EQ(6000, sid.InitialAltitude());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasAnInitialHeading)
    {
        EXPECT_EQ(55, sid.InitialHeading());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasAHandoffId)
    {
        EXPECT_EQ(3, sid.HandoffId());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasAHandoff)
    {
        EXPECT_TRUE(sid.HasHandoff());
    }

    TEST_F(StandardInstrumentDepartureTest, ItDoesntHaveAHandoff)
    {
        StandardInstrumentDeparture sid2(3, 3, "WOTAN1Z", 6000, 55, 0);
        EXPECT_FALSE(sid2.HasHandoff());
    }

    TEST_F(StandardInstrumentDepartureTest, ItHasPrenotes)
    {
        EXPECT_EQ(std::set<int>({1, 2}), sid.Prenotes());
    }

    TEST_F(StandardInstrumentDepartureTest, ItDefaultsToNoPrenotes)
    {
        EXPECT_TRUE(sidWithDefaults.Prenotes().empty());
    }
} // namespace UKControllerPluginTest::Sid
