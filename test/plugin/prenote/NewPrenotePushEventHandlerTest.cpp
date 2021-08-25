#include "prenote/NewPrenotePushEventHandler.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"
#include "time/ParseTimeStrings.h"

using UKControllerPlugin::Prenote::NewPrenotePushEventHandler;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;
using UKControllerPlugin::Time::ParseTimeString;

namespace UKControllerPluginTest::Prenote {
    class NewPrenotePushEventHandlerTest : public testing::Test
    {
        public:
        NewPrenotePushEventHandlerTest() : messages(std::make_shared<PrenoteMessageCollection>()), handler(messages){};

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        static PushEvent MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
        {
            nlohmann::json eventData{
                {"id", 1},
                {"callsign", "BAW123"},
                {"departure_airfield", "EGGD"},
                {"departure_sid", "WOTAN1Z"},
                {"destination_airfield", "EGLL"},
                {"sending_controller", 1},
                {"target_controller", 2},
                {"expires_at", "2021-08-23 14:00:00"}};
            eventData.update(overridingData);

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"prenote-message.received", "test", eventData, eventData.dump()};
        };

        std::shared_ptr<PrenoteMessageCollection> messages;
        NewPrenotePushEventHandler handler;
    };

    TEST_F(NewPrenotePushEventHandlerTest, ItHasSubscriptions)
    {
        auto subs =
            std::set<PushEventSubscription>{{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.received"}};

        EXPECT_EQ(subs, handler.GetPushEventSubscriptions());
    }

    TEST_F(NewPrenotePushEventHandlerTest, ItAddsPrenoteFromMessage)
    {
        this->handler.ProcessPushEvent(MakePushEvent());

        EXPECT_EQ(1, this->messages->Count());
        auto message = this->messages->GetById(1);
        EXPECT_EQ(1, message->GetId());
        EXPECT_EQ("BAW123", message->GetCallsign());
        EXPECT_EQ("EGGD", message->GetDepartureAirfield());
        EXPECT_EQ("WOTAN1Z", message->GetSid());
        EXPECT_EQ("EGLL", message->GetDestinationAirfield());
        EXPECT_EQ(1, message->GetSendingControllerId());
        EXPECT_EQ(2, message->GetTargetControllerId());
        EXPECT_EQ(ParseTimeString("2021-08-23 14:00:00"), message->GetExpiresAt());
        EXPECT_FALSE(message->IsAcknowledged());
    }

    TEST_F(NewPrenotePushEventHandlerTest, ItAddsPrenoteFromMessageWithOptionalData)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json{
            {"departure_sid", nlohmann::json::value_t::null},
            {"destination_airfield", nlohmann::json::value_t::null},
        }));

        EXPECT_EQ(1, this->messages->Count());
        auto message = this->messages->GetById(1);
        EXPECT_EQ(1, message->GetId());
        EXPECT_EQ("BAW123", message->GetCallsign());
        EXPECT_EQ("EGGD", message->GetDepartureAirfield());
        EXPECT_EQ("", message->GetSid());
        EXPECT_EQ("", message->GetDestinationAirfield());
        EXPECT_EQ(1, message->GetSendingControllerId());
        EXPECT_EQ(2, message->GetTargetControllerId());
        EXPECT_EQ(ParseTimeString("2021-08-23 14:00:00"), message->GetExpiresAt());
        EXPECT_FALSE(message->IsAcknowledged());
    }

    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfIdMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "id"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfIdNotInteger)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"id", "abc"}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfCallsignMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "callsign"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfCallsignNotString)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"callsign", 123}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDepartureAirfieldMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "departure_airfield"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDepartureAirfieldNotString)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"departure_airfield", 123}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDepartureSidMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "departure_sid"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDepartureSidNotNullOrString)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"departure_sid", 123}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDestinationAirfieldMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "destination_airfield"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfDestinationAirfieldNotString)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"destination_airfield", 123}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfSendingControllerMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "sending_controller"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfSendingControllerNotInteger)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"sending_controller", "abc"}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfTargetControllerMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "target_controller"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfTargetControllerNotInteger)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"target_controller", "abc"}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfExpiresAtMissing)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "expires_at"));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfExpiresAtNotString)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"expires_at", 123}})));
        EXPECT_EQ(0, this->messages->Count());
    }
    
    TEST_F(NewPrenotePushEventHandlerTest, ItDoesntAddPrenoteIfExpiresAtNotValidTimestamp)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"expires_at", "123-456"}})));
        EXPECT_EQ(0, this->messages->Count());
    }
} // namespace UKControllerPluginTest::Prenote
