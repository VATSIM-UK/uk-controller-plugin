#include "pch/pch.h"
#include "hold/ManagedHold.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldingData.h"

using UKControllerPlugin::Hold::ManagedHold;
using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldingData;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class ManagedHoldTest : public Test
        {
            public:

                ManagedHoldTest()
                    : hold(holdData)
                {

                }

                const UKControllerPlugin::Hold::HoldingData holdData = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, 0 };
                UKControllerPlugin::Hold::ManagedHold hold;
                const UKControllerPlugin::Hold::HoldingAircraft aircraft = {
                    "BAW123",
                    std::chrono::system_clock::now()
                };
        };

        TEST_F(ManagedHoldTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->hold.CountHoldingAircraft());
        }

        TEST_F(ManagedHoldTest, ItSetsHoldParameters)
        {
            EXPECT_EQ(this->holdData, this->hold.holdParameters);
        }

        TEST_F(ManagedHoldTest, ItAddsHoldingAircraft)
        {
            this->hold.AddHoldingAircraft(aircraft);
            EXPECT_EQ(1, this->hold.CountHoldingAircraft());
        }

        TEST_F(ManagedHoldTest, ItRemovesHoldingAircraft)
        {
            this->hold.AddHoldingAircraft(aircraft);
            this->hold.RemoveHoldingAircraft(aircraft);
            EXPECT_EQ(0, this->hold.CountHoldingAircraft());
        }

        TEST_F(ManagedHoldTest, ItRemovesHoldingAircraftByCallsign)
        {
            this->hold.AddHoldingAircraft(aircraft);
            this->hold.RemoveHoldingAircraft("BAW123");
            EXPECT_EQ(0, this->hold.CountHoldingAircraft());
        }

        TEST_F(ManagedHoldTest, ItHandlesRemovingNonHoldingAircraft)
        {
            EXPECT_NO_THROW(this->hold.RemoveHoldingAircraft(aircraft));
        }

        TEST_F(ManagedHoldTest, EqualityReturnsTrueIfSameIdentifiers)
        {
            ManagedHold hold2({ 1, "NOT TIMBA", "TIMBA", 8000, 15000, 209, 0 });
            EXPECT_TRUE(this->hold == hold2);
        }

        TEST_F(ManagedHoldTest, EqualityReturnsFalseIfDifferentIdentifiers)
        {
            ManagedHold hold2({ 2, "TIMBA", "TIMBA", 8000, 15000, 209, 0 });
            EXPECT_FALSE(this->hold == hold2);
        }

        TEST_F(ManagedHoldTest, ItChecksAircraftInHoldByStruct)
        {
            this->hold.AddHoldingAircraft(aircraft);
            EXPECT_TRUE(this->hold.HasAircraft(aircraft));
        }

        TEST_F(ManagedHoldTest, ItChecksAircraftInHoldByString)
        {
            this->hold.AddHoldingAircraft(aircraft);
            EXPECT_TRUE(this->hold.HasAircraft("BAW123"));
        }

        TEST_F(ManagedHoldTest, ItChecksAircraftInHoldByStringReturnsFalseIfNotHolding)
        {
            this->hold.AddHoldingAircraft(aircraft);
            EXPECT_FALSE(this->hold.HasAircraft("BAW124"));
        }

    }  // namespace Hold
}  // namespace UKControllerPluginTest