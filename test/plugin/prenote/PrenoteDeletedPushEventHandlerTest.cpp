#include "controller/ControllerPosition.h"
#include "prenote/PrenoteDeletedPushEventHandler.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteMessageEventHandlerCollection.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteDeletedPushEventHandler;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::PrenoteMessageEventHandlerCollection;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::Prenote {
    class PrenoteDeletedPushEventHandlerHandlerTest : public testing::Test
    {
        public:
        PrenoteDeletedPushEventHandlerHandlerTest()
            : messages(std::make_shared<PrenoteMessageCollection>()), handler(messages, eventHandlers)
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            this->messages->Add(std::make_shared<PrenoteMessage>(
                1,
                +"BAW123",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                std::chrono::system_clock::now()));
            mockHandler = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
            eventHandlers.AddHandler(mockHandler);
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

        std::shared_ptr<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>> mockHandler;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<PrenoteMessageCollection> messages;
        PrenoteMessageEventHandlerCollection eventHandlers;
        PrenoteDeletedPushEventHandler handler;
    };

    TEST_F(PrenoteDeletedPushEventHandlerHandlerTest, ItHasSubscriptions)
    {
        auto subs = std::set<PushEventSubscription>{{PushEventSubscription::SUB_TYPE_EVENT, "prenote-message.deleted"}};

        EXPECT_EQ(subs, handler.GetPushEventSubscriptions());
    }

    TEST_F(PrenoteDeletedPushEventHandlerHandlerTest, ItRemovesPrenoteFromMessage)
    {
        EXPECT_CALL(*mockHandler, MessageCancelled(testing::_)).Times(1);
        this->handler.ProcessPushEvent(MakePushEvent());
        EXPECT_EQ(nullptr, this->messages->GetById(1));
    }

    TEST_F(PrenoteDeletedPushEventHandlerHandlerTest, ItHandlesMissingIdFromMessage)
    {
        EXPECT_CALL(*mockHandler, MessageCancelled(testing::_)).Times(0);
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object(), "id"));
        EXPECT_NE(nullptr, this->messages->GetById(1));
    }

    TEST_F(PrenoteDeletedPushEventHandlerHandlerTest, ItHandlesIdNotIntegerFromMessage)
    {
        EXPECT_CALL(*mockHandler, MessageCancelled(testing::_)).Times(0);
        this->handler.ProcessPushEvent(MakePushEvent(nlohmann::json::object({{"id", "abc"}})));
        EXPECT_NE(nullptr, this->messages->GetById(1));
    }

    TEST_F(PrenoteDeletedPushEventHandlerHandlerTest, ItHandlesPrenoteNotFoundToRemove)
    {
        EXPECT_CALL(*mockHandler, MessageCancelled(testing::_)).Times(0);
        this->messages->Remove(1);
        EXPECT_NO_THROW(this->handler.ProcessPushEvent(MakePushEvent()));
    }
} // namespace UKControllerPluginTest::Prenote
