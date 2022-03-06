#include "api/ApiException.h"
#include "hold/HoldingAircraft.h"
#include "hold/HoldManager.h"
#include "hold/ProximityHold.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using ::testing::Throw;
using UKControllerPlugin::Api::ApiException;
using UKControllerPlugin::Hold::HoldingAircraft;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPluginTest::Api::MockApiInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::TaskManager::MockTaskRunnerInterface;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldManagerTest : public Test
        {
            public:
            HoldManagerTest(void) : manager(mockApi, mockTaskRunner)
            {

                ON_CALL(mockFlightplan1, GetCallsign()).WillByDefault(Return("BAW123"));

                ON_CALL(mockFlightplan2, GetCallsign()).WillByDefault(Return("EZY234"));

                this->manager.AssignAircraftToHold("EZY234", "TIMBA", false);
            }

            NiceMock<MockApiInterface> mockApi;
            NiceMock<MockTaskRunnerInterface> mockTaskRunner;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan1;
            NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan2;
            NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
            NiceMock<MockEuroscopePluginLoopbackInterface> mockPlugin;
            HoldManager manager;
        };

        TEST_F(HoldManagerTest, AddingAircraftToProximityHoldsCreatesNewInstance)
        {
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "LAM"));

            EXPECT_EQ(
                this->manager.GetHoldingAircraft("BAW123")->GetNoHoldAssigned(),
                this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());

            EXPECT_EQ(1, this->manager.GetAircraftForHold("LAM").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("LAM").cbegin())->GetCallsign());
            EXPECT_EQ(1, this->manager.GetHoldingAircraft("BAW123")->GetProximityHolds().size());
            EXPECT_NE(nullptr, this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("LAM"));
            EXPECT_EQ("BAW123", this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("LAM")->Callsign());
            EXPECT_EQ("LAM", this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("LAM")->Navaid());
        }

        TEST_F(HoldManagerTest, AddingAircraftToProximityHoldsUpdatesExistingInstance)
        {
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "LAM"));
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "BNN"));

            EXPECT_EQ(
                this->manager.GetHoldingAircraft("BAW123")->GetNoHoldAssigned(),
                this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());

            EXPECT_EQ(1, this->manager.GetAircraftForHold("LAM").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("LAM").cbegin())->GetCallsign());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("BNN").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("BNN").cbegin())->GetCallsign());

            EXPECT_EQ(2, this->manager.GetHoldingAircraft("BAW123")->GetProximityHolds().size());
            EXPECT_NE(nullptr, this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("BNN"));
            EXPECT_EQ("BAW123", this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("BNN")->Callsign());
            EXPECT_EQ("BNN", this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("BNN")->Navaid());
        }

        TEST_F(HoldManagerTest, AssigningAircraftToHoldCreatesNewInstance)
        {
            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->manager.AssignAircraftToHold("BAW123", "LAM", false);

            EXPECT_EQ(0, this->manager.GetHoldingAircraft("BAW123")->GetProximityHolds().size());
            EXPECT_EQ("LAM", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("LAM").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("LAM").cbegin())->GetCallsign());
        }

        TEST_F(HoldManagerTest, AssigningAircraftToHoldUpdatesExistingAndRemovesFromPreviousHoldIfNotAffiliated)
        {
            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->manager.AssignAircraftToHold("BAW123", "LAM", false);
            this->manager.AssignAircraftToHold("BAW123", "BNN", false);

            EXPECT_EQ(0, this->manager.GetHoldingAircraft("BAW123")->GetProximityHolds().size());
            EXPECT_EQ("BNN", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("BNN").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("BNN").cbegin())->GetCallsign());
            EXPECT_EQ(0, this->manager.GetAircraftForHold("LAM").size());
        }

        TEST_F(HoldManagerTest, AssigningAircraftToHoldUpdatesExistingAndRemovesFromPreviousHoldIfNotProximity)
        {
            EXPECT_CALL(this->mockApi, AssignAircraftToHold(_, _)).Times(0);

            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "LAM"));
            this->manager.AssignAircraftToHold("BAW123", "LAM", false);
            this->manager.AssignAircraftToHold("BAW123", "BNN", false);

            EXPECT_EQ("BNN", this->manager.GetHoldingAircraft("BAW123")->GetAssignedHold());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("BNN").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("BNN").cbegin())->GetCallsign());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("LAM").size());
            EXPECT_EQ("BAW123", (*this->manager.GetAircraftForHold("LAM").cbegin())->GetCallsign());
        }

        TEST_F(HoldManagerTest, AssigningAircraftToHoldUpdatesApiIfSelected)
        {
            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "LAM")).Times(1);

            this->manager.AssignAircraftToHold("BAW123", "LAM", true);
        }

        TEST_F(HoldManagerTest, AssigningAircraftToHoldHandlesApiExceptions)
        {
            EXPECT_CALL(this->mockApi, AssignAircraftToHold("BAW123", "LAM"))
                .Times(1)
                .WillOnce(Throw(ApiException("Test")));

            this->manager.AssignAircraftToHold("BAW123", "LAM", true);
        }

        TEST_F(HoldManagerTest, GetAircraftForHoldReturnsEmptySetIfNone)
        {
            EXPECT_EQ(0, this->manager.GetAircraftForHold("LAM").size());
        }

        TEST_F(HoldManagerTest, UnassigningAircraftFromHoldHandlesIfNotHolding)
        {
            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            EXPECT_NO_THROW(this->manager.UnassignAircraftFromHold("BAW123", false));
        }

        TEST_F(HoldManagerTest, UnassigningAircraftRemovesAircraftFromHoldEntirelyIfNoProximity)
        {
            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            this->manager.UnassignAircraftFromHold("EZY234", false);
            EXPECT_EQ(this->manager.invalidAircraft, this->manager.GetHoldingAircraft("EZY234"));
            EXPECT_EQ(0, this->manager.GetAircraftForHold("TIMBA").size());
        }

        TEST_F(HoldManagerTest, UnassigningAircraftKeepsAircraftInHoldIfProximity)
        {
            EXPECT_CALL(this->mockApi, UnassignAircraftHold(_)).Times(0);

            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("EZY234", "TIMBA"));
            this->manager.UnassignAircraftFromHold("EZY234", false);
            EXPECT_EQ(
                this->manager.GetHoldingAircraft("EZY234")->GetNoHoldAssigned(),
                this->manager.GetHoldingAircraft("EZY234")->GetAssignedHold());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("TIMBA").size());
            EXPECT_EQ("EZY234", (*this->manager.GetAircraftForHold("TIMBA").cbegin())->GetCallsign());
        }

        TEST_F(HoldManagerTest, UnassigningAircraftUpdatesApiHoldIfRequired)
        {
            EXPECT_CALL(this->mockApi, UnassignAircraftHold("EZY234")).Times(1);

            this->manager.UnassignAircraftFromHold("EZY234", true);
        }

        TEST_F(HoldManagerTest, UnassigningAircraftHandlesApiException)
        {
            EXPECT_CALL(this->mockApi, UnassignAircraftHold("EZY234")).Times(1).WillOnce(Throw(ApiException("Test")));

            EXPECT_NO_THROW(this->manager.UnassignAircraftFromHold("EZY234", true));
        }

        TEST_F(HoldManagerTest, RemoveAircraftFromProximityHoldHandlesNonExistentCallsign)
        {
            EXPECT_NO_THROW(this->manager.RemoveAircraftFromProximityHold("BAW123", "TIMBA"));
        }

        TEST_F(HoldManagerTest, RemoveAircraftFromProximityRemovesAircraftEntirelyIfNoHolds)
        {
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "WILLO"));
            this->manager.RemoveAircraftFromProximityHold("BAW123", "WILLO");
            EXPECT_EQ(this->manager.invalidAircraft, this->manager.GetHoldingAircraft("BAW123"));
            EXPECT_EQ(0, this->manager.GetAircraftForHold("WILLO").size());
        }

        TEST_F(HoldManagerTest, RemoveAircraftFromProximityRetainsAircraftIfHoldingSomewhere)
        {
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "MAY"));
            this->manager.AddAircraftToProximityHold(std::make_shared<ProximityHold>("BAW123", "WILLO"));
            this->manager.RemoveAircraftFromProximityHold("BAW123", "MAY");

            EXPECT_EQ("BAW123", this->manager.GetHoldingAircraft("BAW123")->GetCallsign());
            EXPECT_EQ(1, this->manager.GetHoldingAircraft("BAW123")->GetProximityHolds().size());
            EXPECT_NE(nullptr, this->manager.GetHoldingAircraft("BAW123")->GetProximityHold("WILLO"));
            EXPECT_EQ(0, this->manager.GetAircraftForHold("MAY").size());
            EXPECT_EQ(1, this->manager.GetAircraftForHold("WILLO").size());
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
