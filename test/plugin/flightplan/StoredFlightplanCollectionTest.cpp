#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"

using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;

namespace UKControllerPluginTest {
    namespace Flightplan {

        TEST(StoredFlightplanCollection, GetFlightplanForCallsignThrowsExceptionIfNotFound)
        {
            StoredFlightplanCollection collection;
            EXPECT_THROW(collection.GetFlightplanForCallsign("BAW123"), std::out_of_range);
        }

        TEST(StoredFlightplanCollection, HasFlightplanForCallsignReturnsFalseIfNotFound)
        {
            StoredFlightplanCollection collection;
            EXPECT_FALSE(collection.HasFlightplanForCallsign("BAW123"));
        }

        TEST(StoredFlightplanCollection, HasFlightplanForCallsignReturnsTrueIfFound)
        {
            StoredFlightplanCollection collection;
            collection.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGLL"));
            EXPECT_TRUE(collection.HasFlightplanForCallsign("BAW123"));
        }

        TEST(StoredFlightplanCollection, UpdatePlanUpdatesPlanIfAlreadyExists)
        {
            StoredFlightplanCollection collection;
            collection.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGLL"));
            collection.UpdatePlan(StoredFlightplan("BAW123", "EDDM", "EGNM"));

            StoredFlightplan retrievedPlan = collection.GetFlightplanForCallsign("BAW123");
            EXPECT_TRUE(retrievedPlan.GetCallsign() == "BAW123");
            EXPECT_TRUE(retrievedPlan.GetOrigin() == "EDDM");
            EXPECT_TRUE(retrievedPlan.GetDestination() == "EGNM");
        }

        TEST(StoredFlightplanCollection, RemovePlanByCallsignRemovesAPlan)
        {
            StoredFlightplanCollection collection;
            collection.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGLL"));

            EXPECT_TRUE(collection.HasFlightplanForCallsign("BAW123"));
            collection.RemovePlanByCallsign("BAW123");
            EXPECT_FALSE(collection.HasFlightplanForCallsign("BAW123"));
        }

        TEST(StoredFlightplanCollection, RemoveTimedOutPlansRemovesPlansThatHaveTimedOut)
        {
            StoredFlightplanCollection collection;
            StoredFlightplan timedOut = StoredFlightplan("BAW456", "EGKK", "EGLL");
            timedOut.SetTimeout(-50);

            collection.UpdatePlan(StoredFlightplan("BAW123", "EGKK", "EGLL"));
            collection.UpdatePlan(StoredFlightplan(timedOut));

            collection.RemoveTimedOutPlans();

            EXPECT_TRUE(collection.HasFlightplanForCallsign("BAW123"));
            EXPECT_FALSE(collection.HasFlightplanForCallsign("BAW456"));
        }

    }  // namespace Flightplan
}  // namespace UKControllerPluginTest
