#include "controller/ControllerPosition.h"
#include "prenote/PrenoteAcknowledgedPushEventHandler.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteAcknowledgedPushEventHandler;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::Prenote {
    class PrenoteAcknowledgedPushEventHandlerTest : public testing::Test
    {
        public:
        PrenoteAcknowledgedPushEventHandlerTest()
            : messages(std::make_shared<PrenoteMessageCollection>()), handler(messages)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            this->messages->Add(std::make_shared<PrenoteMessage>(
                1,
                "BAW123",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                std::chrono::system_clock::now()));
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        static PushEvent MakePushEvent(
            const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
        {
            nlohmann::json eventData{{"id", 1}};
            eventData.update(overridingData);

            if (!keyToRemove.empty()) {
                eventData.erase(eventData.find(keyToRemove));
            }

            return {"prenote-message.received", "test", eventData, eventData.dump()};
        };

        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<PrenoteMessageCollection> messages;
        PrenoteAcknowledgedPushEventHandler handler;
    };

    TEST_F(PrenoteAcknowledgedPushEventHandlerTest, ItHasSubscriptions)
    {
        auto subs =
            std::set<PushEventSubscription>{{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.acknowledged"}};

        EXPECT_EQ(subs, handler.GetPushEventSubscriptions());
    }

    TEST_F(PrenoteAcknowledgedPushEventHandlerTest, ItAcknowledgesPrenoteFromMessage)
    {
        this->handler.ProcessPushEvent(MakePushEvent());
        EXPECT_TRUE(this->messages->GetById(1)->IsAcknowledged());
    }

    TEST_F(PrenoteAcknowledgedPushEventHandlerTest, ItHandlesMissingIdFromMessage)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "id"));
        EXPECT_FALSE(this->messages->GetById(1)->IsAcknowledged());
    }

    TEST_F(PrenoteAcknowledgedPushEventHandlerTest, ItHandlesIdNotIntegerFromMessage)
    {
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"id", "abc"}})));
        EXPECT_FALSE(this->messages->GetById(1)->IsAcknowledged());
    }

    TEST_F(PrenoteAcknowledgedPushEventHandlerTest, ItHandlesPrenoteNotFoundToAcknowledge)
    {
        this->messages->Remove(1);
        EXPECT_NO_THROW(this->handler.ProcessPushEvent(MakePushEvent()));
    }
} // namespace UKControllerPluginTest::Prenote
