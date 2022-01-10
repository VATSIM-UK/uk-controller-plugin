#include "hold/AssignHoldCommand.h"
#include "hold/HoldManager.h"

using UKControllerPlugin::Hold::AssignHoldCommand;
using UKControllerPlugin::Hold::HoldManager;

namespace UKControllerPluginTest::Hold {
    class AssignedHoldCommandTest : public testing::Test
    {
        public:
        AssignedHoldCommandTest() : holdManager(api, taskRunner), command(holdManager, plugin)
        {
            mockFlightplan = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();

            ON_CALL(*mockFlightplan, GetCallsign()).WillByDefault(testing::Return("BAW123"));
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>> mockFlightplan;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<TaskManager::MockTaskRunnerInterface> taskRunner;
        HoldManager holdManager;
        AssignHoldCommand command;
    };

    TEST_F(AssignedHoldCommandTest, ItReturnsNothingOnBadCommand)
    {
        EXPECT_FALSE(command.ProcessCommand(".ukcp fooby"));
    }

    TEST_F(AssignedHoldCommandTest, ItRemovesHoldFromSelectedAircraft)
    {
        EXPECT_CALL(plugin, GetSelectedFlightplan).Times(1).WillOnce(testing::Return(mockFlightplan));

        EXPECT_CALL(api, UnassignAircraftHold("BAW123")).Times(1);

        holdManager.AssignAircraftToHold("BAW123", "BNN", false);
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
        EXPECT_TRUE(command.ProcessCommand(".ukcp nohold"));
        EXPECT_EQ(0, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItDoesntRemoveHoldIfNoSelectedFlightplan)
    {
        EXPECT_CALL(plugin, GetSelectedFlightplan).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_CALL(api, UnassignAircraftHold(testing::_)).Times(0);

        holdManager.AssignAircraftToHold("BAW123", "BNN", false);
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
        EXPECT_FALSE(command.ProcessCommand(".ukcp nohold"));
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItRemovesHoldFromSpecifiedAircraft)
    {
        EXPECT_CALL(plugin, GetFlightplanForCallsign("BAW123")).Times(1).WillOnce(testing::Return(mockFlightplan));

        EXPECT_CALL(api, UnassignAircraftHold("BAW123")).Times(1);

        holdManager.AssignAircraftToHold("BAW123", "BNN", false);
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
        EXPECT_TRUE(command.ProcessCommand(".ukcp nohold BAW123"));
        EXPECT_EQ(0, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItDoesntRemoveHoldSpecifiedCallsignNotFound)
    {
        EXPECT_CALL(plugin, GetFlightplanForCallsign("BAW123")).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_CALL(api, UnassignAircraftHold(testing::_)).Times(0);

        holdManager.AssignAircraftToHold("BAW123", "BNN", false);
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
        EXPECT_FALSE(command.ProcessCommand(".ukcp nohold BAW123"));
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItAssignsHoldForSelectedFlightplan)
    {
        EXPECT_CALL(plugin, GetSelectedFlightplan).Times(1).WillOnce(testing::Return(mockFlightplan));

        EXPECT_CALL(api, AssignAircraftToHold("BAW123", "BNN")).Times(1);

        EXPECT_TRUE(command.ProcessCommand(".ukcp hold BNN"));
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItDoesntAssignHoldNoSelectedFlightplan)
    {
        EXPECT_CALL(plugin, GetSelectedFlightplan).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_CALL(api, AssignAircraftToHold).Times(0);

        EXPECT_FALSE(command.ProcessCommand(".ukcp hold BNN"));
        EXPECT_EQ(0, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItAssignsHoldForSpecifiedFlightplan)
    {
        EXPECT_CALL(plugin, GetFlightplanForCallsign("BAW123")).Times(1).WillOnce(testing::Return(mockFlightplan));

        EXPECT_CALL(api, AssignAircraftToHold("BAW123", "BNN")).Times(1);

        EXPECT_TRUE(command.ProcessCommand(".ukcp hold BNN BAW123"));
        EXPECT_EQ(1, holdManager.GetAircraftForHold("BNN").size());
    }

    TEST_F(AssignedHoldCommandTest, ItDoesntAssignHoldSpecifiedFlightplanNotFound)
    {
        EXPECT_CALL(plugin, GetFlightplanForCallsign("BAW123")).Times(1).WillOnce(testing::Return(nullptr));

        EXPECT_CALL(api, AssignAircraftToHold).Times(0);

        EXPECT_FALSE(command.ProcessCommand(".ukcp hold BNN BAW123"));
        EXPECT_EQ(0, holdManager.GetAircraftForHold("BNN").size());
    }
} // namespace UKControllerPluginTest::Hold
