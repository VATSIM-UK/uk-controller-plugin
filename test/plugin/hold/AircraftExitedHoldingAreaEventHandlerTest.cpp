#include "hold/AircraftExitedHoldingAreaEventHandler.h"
#include "hold/HoldManager.h"
#include "hold/HoldingAircraft.h"
#include "hold/ProximityHold.h"
#include "navaids/Navaid.h"
#include "navaids/NavaidCollection.h"
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Hold::AircraftExitedHoldingAreaEventHandler;
using UKControllerPlugin::Hold::HoldManager;
using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPlugin::Navaids::Navaid;
using UKControllerPlugin::Navaids::NavaidCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::Hold {
    class AircraftExitedHoldingAreaEventHandlerTest : public testing::Test
    {

        public:
        AircraftExitedHoldingAreaEventHandlerTest()
            : navaid({1, "BNN", EuroScopePlugIn::CPosition()}), holdManager(api, taskRunner),
              handler(holdManager, navaids)
        {
            this->navaids.AddNavaid(navaid);
            this->holdManager.AddAircraftToProximityHold(
                std::make_shared<ProximityHold>("BAW123", "BNN", std::chrono::system_clock::now()));
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        [[nodiscard]] static auto MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> PushEvent
        {
            nlohmann::json eventData{{"callsign", "BAW123"}, {"navaid_id", 1}};
            if (overridingData.is_object()) {
                eventData.update(overridingData);
            } else {
                eventData = overridingData;
            }

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"hold.area-exited", "test", eventData, eventData.dump()};
        };

        Navaid navaid;
        NavaidCollection navaids;
        testing::NiceMock<Api::MockApiInterface> api;
        testing::NiceMock<TaskManager::MockTaskRunnerInterface> taskRunner;
        HoldManager holdManager;
        AircraftExitedHoldingAreaEventHandler handler;
    };

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHasPushEventSubscriptions)
    {
        std::set<PushEventSubscription> expected = {
            {PushEventSubscription::SUB_TYPE_EVENT,
             "hold"
             ".area-exited"}};
        EXPECT_EQ(expected, handler.GetPushEventSubscriptions());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItProcessesPushEvents)
    {
        handler.ProcessPushEvent(MakePushEvent());
        EXPECT_EQ(0, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventDataNotObject)
    {
        handler.ProcessPushEvent({"hold.area-entered", "test", nlohmann::json::array(), "[]"});
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventMissingCallsign)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "callsign"));
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAString)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"callsign", 123}})));
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventMissingNavaidId)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "navaid_id"));
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAnInteger)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"navaid_id", "abc"}})));
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }

    TEST_F(AircraftExitedHoldingAreaEventHandlerTest, ItHandlesPushEventNavaidIdNotAValidNavaid)
    {
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"navaid_id", 123}})));
        EXPECT_EQ(1, holdManager.CountHoldingAircraft());
    }
} // namespace UKControllerPluginTest::Hold
