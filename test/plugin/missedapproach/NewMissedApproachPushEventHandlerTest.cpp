#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachAudioAlert.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/MissedApproachOptions.h"
#include "missedapproach/MissedApproachTriggeredMessage.h"
#include "missedapproach/NewMissedApproachPushEventHandler.h"
#include "ownership/AirfieldServiceProviderCollection.h"
#include "time/ParseTimeStrings.h"
#include "time/SystemClock.h"

using testing::NiceMock;
using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachAudioAlert;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::MissedApproachOptions;
using UKControllerPlugin::MissedApproach::MissedApproachTriggeredMessage;
using UKControllerPlugin::MissedApproach::NewMissedApproachPushEventHandler;
using UKControllerPlugin::Ownership::AirfieldServiceProviderCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Time::ParseTimeString;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;
using UKControllerPluginTest::Euroscope::MockEuroscopePluginLoopbackInterface;
using UKControllerPluginTest::Windows::MockWinApi;

namespace UKControllerPluginTest::MissedApproach {
    class NewMissedApproachPushEventHandlerTest : public testing::Test
    {
        public:
        NewMissedApproachPushEventHandlerTest()
            : missed1(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true)),
              missed2(std::make_shared<class MissedApproach>(2, "BAW456", std::chrono::system_clock::now(), true)),
              collection(std::make_shared<MissedApproachCollection>()),
              options(std::make_shared<MissedApproachOptions>()),
              audioAlert(std::make_shared<MissedApproachAudioAlert>(options, plugin, ownership, windows)),
              handler(collection, audioAlert, mockIntegration)
        {
            SetTestNow(ParseTimeString("2021-08-23 13:55:00"));
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        static PushEvent MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
        {
            nlohmann::json eventData{{"id", 1}, {"callsign", "BAW123"}, {"expires_at", "2021-08-23 14:00:00"}};
            if (overridingData.is_object()) {
                eventData.update(overridingData);
            } else {
                eventData = overridingData;
            }

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"missed-approach.created", "test", eventData, eventData.dump()};
        };

        AirfieldServiceProviderCollection ownership;
        NiceMock<Integration::MockOutboundIntegrationEventHandler> mockIntegration;
        NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> plugin;
        NiceMock<MockWinApi> windows;
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
        std::shared_ptr<MissedApproachCollection> collection;
        std::shared_ptr<MissedApproachOptions> options;
        std::shared_ptr<MissedApproachAudioAlert> audioAlert;
        NewMissedApproachPushEventHandler handler;
    };

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHasPushEvents)
    {
        std::set<PushEventSubscription> expected = {{PushEventSubscription::SUB_TYPE_EVENT, "missed-approach.created"}};
        EXPECT_EQ(expected, handler.GetPushEventSubscriptions());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItAddsAMissedApproach)
    {
        auto expectedMessage =
            std::make_shared<MissedApproachTriggeredMessage>("BAW123", false, ParseTimeString("2021-08-23 14:00:00"));
        EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        handler.ProcessPushEvent(MakePushEvent());
        EXPECT_EQ(1, collection->Count());

        auto message = collection->Get("BAW123");
        EXPECT_NE(nullptr, message);
        EXPECT_EQ(1, message->Id());
        EXPECT_EQ("BAW123", message->Callsign());
        EXPECT_EQ(ParseTimeString("2021-08-23 14:00:00"), message->ExpiresAt());
        EXPECT_FALSE(message->CreatedByUser());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItDoesntOverwriteExistingMissedApproach)
    {
        collection->Add(
            std::make_shared<class MissedApproach>(1, "BAW123", ParseTimeString("2021-08-23 14:05:00"), true));

        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);

        handler.ProcessPushEvent(MakePushEvent());
        EXPECT_EQ(1, collection->Count());

        auto message = collection->Get("BAW123");
        EXPECT_NE(nullptr, message);
        EXPECT_EQ("BAW123", message->Callsign());
        EXPECT_EQ(ParseTimeString("2021-08-23 14:05:00"), message->ExpiresAt());
        EXPECT_TRUE(message->CreatedByUser());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesMessageNotObject)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::array()));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesMissingIdInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "id"));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesIdNotIntegerInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json{{"id", "123"}}));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesMissingCallsignInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "callsign"));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesCallsignNotStringInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json{{"callsign", 123}}));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesExpiresAtMissingInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "expires_at"));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesExpiresAtNotStringInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json{{"expires_at", 123}}));
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(NewMissedApproachPushEventHandlerTest, ItHandlesExpiresAtNotValidTimestampInMessage)
    {
        EXPECT_CALL(this->mockIntegration, SendEvent(testing::_)).Times(0);
        handler.ProcessPushEvent(MakePushEvent(nlohmann::json{{"expires_at", "abc"}}));
        EXPECT_EQ(0, collection->Count());
    }
} // namespace UKControllerPluginTest::MissedApproach
