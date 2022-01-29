#include "hold/AircraftEnteredHoldingAreaEventHandler.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/ProximityHold.h"
#include "navaids/Navaid.h"
#include "navaids/NavaidCollection.h"
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"
#include "time/SystemClock.h"
#include "time/ParseTimeStrings.h"

using UKControllerPlugin::Hold::AircraftEnteredHoldingAreaEventHandler;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Time::ParseIsoZuluString;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Hold {
    class AircraftEnteredHoldingAreaEventHandlerTest : public testing::Test
    {

        public:
        AircraftEnteredHoldingAreaEventHandlerTest()
            : navaid({1, "BNN", EuroScopePlugIn::CPosition()}), holdManager(api, taskRunner),
              handler(holdManager, navaids)
        {
            SetTestNow(TimeNow());
            navaids.AddNavaid(navaid);
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        [[nodiscard]] static auto MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> PushEvent
        {
            nlohmann::json eventData{
                {"callsign", "BAW123"}, {"navaid_id", 1}, {"entered_at", "2021-01-09T01:02:03.000000Z"}};
            if (overridingData.is_object()) {
                eventData.update(overridingData);
            } else {
                eventData = overridingData;
            }

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"hold.area-entered", "test", eventData, eventData.dump()};
        };

        Navaid navaid;
        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<TaskManager::MockTaskRunnerInterface> taskRunner;
        HoldManager holdManager;
        NavaidCollection navaids;
        AircraftEnteredHoldingAreaEventHandler handler;
    };

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHasPushEventSubscriptions)
    {
        std::set<PushEventSubscription> expected = {
            {PushEventSubscription::SUB_TYPE_EVENT,
             "hold"
             ".area-entered"}};
        EXPECT_EQ(expected, handler.GetPushEventSubscriptions());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItProcessesPushEvents)
    {
        handler.ProcessPushEvent(MakePushEvent());
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
        const auto holdingAircraft = holdManager.GetHoldingAircraft("BAW123");
        EXPECT_NE(nullptr, holdingAircraft);
        const auto proximityHold = holdingAircraft->GetProximityHold("BNN");
        EXPECT_NE(nullptr, proximityHold);
        EXPECT_EQ("BAW123", proximityHold->callsign);
        EXPECT_EQ("BNN", proximityHold->navaid);
        EXPECT_EQ(ParseIsoZuluString("2021-01-09T01:02:03.000000Z"), proximityHold->enteredAt);
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventDataNotObject)
    {
        handler.ProcessPushEvent({"hold.area-entered", "test", nlohmann::json::array(), "[]"});
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventMissingCallsign)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "callsign"));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAString)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"callsign", 123}})));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventMissingNavaidId)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "navaid_id"));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAnInteger)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"navaid_id", "abc"}})));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAValidNavaid)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"navaid_id", 123}})));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventMissingEnteredAt)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "entered_at"));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventEnteredAtNotAString)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"entered_at", 123}})));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftEnteredHoldingAreaEventHandlerTest, ItHandlesPushEventEnteredAtNotAValidTimestamp)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"entered_at", "abc"}})));
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }
} // namespace UKControllerPluginTest::Hold
