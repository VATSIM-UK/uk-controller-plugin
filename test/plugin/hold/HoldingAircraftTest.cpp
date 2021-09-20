#include "hold/HoldingAircraft.h"

using ::testing::Test;
using UKControllerPlugin::Hold::HoldingAircraft;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldingAircraftTest : public Test
        {
            public:
            HoldingAircraftTest()
                : baseAircraft("BAW123", "BNN"), proximityAircraft("BAW123", std::set<std::string>({"BNN", "LAM"}))
            {
            }

            HoldingAircraft baseAircraft;
            HoldingAircraft proximityAircraft;
        };

        TEST_F(HoldingAircraftTest, AssignedConstructionSetsCallsignAssignedHoldAndEntryTime)
        {
            EXPECT_EQ("BAW123", this->baseAircraft.GetCallsign());
            EXPECT_EQ("BNN", this->baseAircraft.GetAssignedHold());

            std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(
                this->baseAircraft.GetAssignedHoldEntryTime() - std::chrono::system_clock::now());

            EXPECT_LT(seconds, std::chrono::seconds(3));
        }

        TEST_F(HoldingAircraftTest, ProximityConstructionSetsCallsignAndProximityHolds)
        {
            EXPECT_EQ("BAW123", this->proximityAircraft.GetCallsign());
            EXPECT_EQ(std::set<std::string>({"BNN", "LAM"}), this->proximityAircraft.GetProximityHolds());
        }

        TEST_F(HoldingAircraftTest, TestItAddsAProximityHold)
        {
            this->baseAircraft.AddProximityHold("TIMBA");
            EXPECT_EQ(std::set<std::string>({"TIMBA"}), this->baseAircraft.GetProximityHolds());
        }

        TEST_F(HoldingAircraftTest, IsInAnyHoldReturnsTrueIfAssigned)
        {
            EXPECT_TRUE(this->baseAircraft.IsInAnyHold());
        }

        TEST_F(HoldingAircraftTest, IsInAnyHoldReturnsTrueIfProximity)
        {
            EXPECT_TRUE(this->proximityAircraft.IsInAnyHold());
        }

        TEST_F(HoldingAircraftTest, IsInHoldReturnsTrueIfHoldMatches)
        {
            EXPECT_TRUE(this->baseAircraft.IsInHold("BNN"));
        }

        TEST_F(HoldingAircraftTest, IsInHoldReturnsFalseIfHoldDoesntMatch)
        {
            EXPECT_FALSE(this->baseAircraft.IsInHold("LAM"));
        }

        TEST_F(HoldingAircraftTest, IsInHoldReturnsFalseIfAircraftNotInHold)
        {
            this->baseAircraft.RemoveAssignedHold();
            EXPECT_FALSE(this->baseAircraft.IsInHold("BNN"));
        }

        TEST_F(HoldingAircraftTest, IsInProximityReturnsTrueIfInProximity)
        {
            EXPECT_TRUE(this->proximityAircraft.IsInHoldProximity("BNN"));
        }

        TEST_F(HoldingAircraftTest, IsInProximityReturnsFalseIfNotInProximity)
        {
            EXPECT_FALSE(this->proximityAircraft.IsInHoldProximity("OCK"));
        }

        TEST_F(HoldingAircraftTest, RemoveAssignedHoldRemovesFromHold)
        {
            this->baseAircraft.RemoveAssignedHold();
            EXPECT_EQ(this->baseAircraft.GetNoHoldAssigned(), this->baseAircraft.GetAssignedHold());
        }

        TEST_F(HoldingAircraftTest, RemoveProximityHoldRemovesProximity)
        {
            this->proximityAircraft.RemoveProximityHold("BNN");
            EXPECT_EQ(std::set<std::string>({"LAM"}), this->proximityAircraft.GetProximityHolds());
        }

        TEST_F(HoldingAircraftTest, RemoveProximityHoldHandlesNotInProximity)
        {
            EXPECT_NO_THROW(this->proximityAircraft.RemoveProximityHold("ABCDEF"));
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
