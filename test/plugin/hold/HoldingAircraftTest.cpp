#include "hold/HoldingAircraft.h"
#include "hold/ProximityHold.h"
#include "time/SystemClock.h"

using ::testing::Test;
using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldingAircraftTest : public Test
        {
            public:
            HoldingAircraftTest()
                : proximity(std::make_shared<ProximityHold>("BAW123", "BNN", TimeNow())), baseAircraft("BAW123", "BNN"),
                  proximityAircraft("BAW123", proximity)
            {
            }

            std::shared_ptr<ProximityHold> proximity;
            HoldingAircraft baseAircraft;
            HoldingAircraft proximityAircraft;
        };

        TEST_F(HoldingAircraftTest, AssignedConstructionSetsCallsignAndAssignedHold)
        {
            EXPECT_EQ("BAW123", this->baseAircraft.GetCallsign());
            EXPECT_EQ("BNN", this->baseAircraft.GetAssignedHold());
        }

        TEST_F(HoldingAircraftTest, ProximityConstructionSetsCallsignAndProximityHolds)
        {
            EXPECT_EQ("BAW123", this->proximityAircraft.GetCallsign());
            EXPECT_EQ(1, this->proximityAircraft.GetProximityHolds().size());
            EXPECT_EQ(proximity, this->proximityAircraft.GetProximityHold("BNN"));
        }

        TEST_F(HoldingAircraftTest, TestItAddsAProximityHold)
        {
            this->baseAircraft.AddProximityHold(proximity);
            EXPECT_EQ(1, this->baseAircraft.GetProximityHolds().size());
            EXPECT_EQ(proximity, this->baseAircraft.GetProximityHold("BNN"));
        }

        TEST_F(HoldingAircraftTest, TestItDoesntDuplicateProximityHolds)
        {
            this->baseAircraft.AddProximityHold(proximity);
            this->baseAircraft.AddProximityHold(proximity);
            this->baseAircraft.AddProximityHold(proximity);
            EXPECT_EQ(1, this->baseAircraft.GetProximityHolds().size());
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
            EXPECT_EQ(0, this->proximityAircraft.GetProximityHolds().size());
        }

        TEST_F(HoldingAircraftTest, RemoveProximityHoldHandlesNotInProximity)
        {
            EXPECT_NO_THROW(this->proximityAircraft.RemoveProximityHold("ABCDEF"));
        }

        TEST_F(HoldingAircraftTest, ItReturnsProximityHoldIfSet)
        {
            EXPECT_NE(nullptr, this->proximityAircraft.GetProximityHold("BNN"));
        }

        TEST_F(HoldingAircraftTest, ItReturnsNullptrIfNoProximityHold)
        {
            EXPECT_EQ(nullptr, this->proximityAircraft.GetProximityHold("ABC"));
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
