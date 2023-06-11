#include "flightplan/StoredFlightplanCollection.h"
#include "squawk/ResetSquawkOnFailedDelete.h"
#include "squawk/SquawkAssignmentDeleteForConspicuityFailedEvent.h"

namespace UKControllerPluginTest::Squawk {
    class ResetSquawkOnFailedDeleteTest : public testing::Test
    {
        protected:
        ResetSquawkOnFailedDeleteTest()
            : flightplan(std::make_shared<Euroscope::MockEuroScopeCFlightPlanInterface>()),
              handler(plugin, storedFlightplans)
        {
            ON_CALL(*flightplan, GetCallsign()).WillByDefault(testing::Return("BAW123"));
        }

        std::shared_ptr<Euroscope::MockEuroScopeCFlightPlanInterface> flightplan;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        UKControllerPlugin::Flightplan::StoredFlightplanCollection storedFlightplans;
        UKControllerPlugin::Squawk::ResetSquawkOnFailedDelete handler;
    };

    TEST_F(ResetSquawkOnFailedDeleteTest, ItResetsTheFlightplanSquawkToPreviousSquawk)
    {
        EXPECT_CALL(*flightplan, SetSquawk("1234")).Times(1);
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan));

        handler.OnEvent(UKControllerPlugin::Squawk::SquawkAssignmentDeleteForConspicuityFailedEvent{"BAW123", "1234"});
    }

    TEST_F(ResetSquawkOnFailedDeleteTest, ItResetsTheStoredFlightplanSquawkToPreviousSquawk)
    {
        UKControllerPlugin::Flightplan::StoredFlightplan storedPlan("BAW123", "EGKK", "EGPH");
        storedPlan.SetPreviouslyAssignedSquawk("2222");
        this->storedFlightplans.UpdatePlan(storedPlan);

        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(flightplan));

        handler.OnEvent(UKControllerPlugin::Squawk::SquawkAssignmentDeleteForConspicuityFailedEvent{"BAW123", "1234"});
        EXPECT_EQ(storedFlightplans.GetFlightplanForCallsign("BAW123").GetPreviouslyAssignedSquawk(), "1234");
    }

    TEST_F(ResetSquawkOnFailedDeleteTest, ItDoesntResetTheFlightplanSquawkIfNoFlightplan)
    {
        UKControllerPlugin::Flightplan::StoredFlightplan storedPlan("BAW123", "EGKK", "EGPH");
        storedPlan.SetPreviouslyAssignedSquawk("2222");
        this->storedFlightplans.UpdatePlan(storedPlan);

        EXPECT_CALL(*flightplan, SetSquawk("1234")).Times(0);
        ON_CALL(plugin, GetFlightplanForCallsign("BAW123")).WillByDefault(testing::Return(nullptr));

        handler.OnEvent(UKControllerPlugin::Squawk::SquawkAssignmentDeleteForConspicuityFailedEvent{"BAW123", "1234"});
        EXPECT_EQ(storedFlightplans.GetFlightplanForCallsign("BAW123").GetPreviouslyAssignedSquawk(), "2222");
    }
} // namespace UKControllerPluginTest::Squawk
