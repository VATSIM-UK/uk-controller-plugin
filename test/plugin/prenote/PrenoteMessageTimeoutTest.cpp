#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteMessageEventHandlerCollection.h"
#include "prenote/PrenoteMessageTimeout.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Prenote::PrenoteMessageEventHandlerCollection;
using UKControllerPlugin::Prenote::PrenoteMessageTimeout;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageTimeoutTest : public ::testing::Test
    {
        public:
        PrenoteMessageTimeoutTest()
            : messages(std::make_shared<PrenoteMessageCollection>()), timeout(messages, eventHandlers)
        {
            SetTestNow(TimeNow());
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            message1 = std::make_shared<PrenoteMessage>(
                1,
                "BAW123",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                TimeNow() + std::chrono::minutes(1));
            message2 = std::make_shared<PrenoteMessage>(
                2,
                "BAW456",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                TimeNow() - std::chrono::seconds(5));
            message3 = std::make_shared<PrenoteMessage>(
                3,
                "BAW456",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                TimeNow() - std::chrono::seconds(5));
            message4 = std::make_shared<PrenoteMessage>(
                4,
                "BAW456",
                "EGGD",
                "BADIM1X",
                "EGLL",
                sendingPosition,
                receivingPosition,
                TimeNow() - std::chrono::seconds(5));

            messages->Add(message1);
            messages->Add(message2);
            messages->Add(message3);
            messages->Add(message4);

            // Prenote event handlers
            mockHandler = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
            eventHandlers.AddHandler(mockHandler);
        }

        std::shared_ptr<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>> mockHandler;
        PrenoteMessageEventHandlerCollection eventHandlers;
        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        std::shared_ptr<PrenoteMessageCollection> messages;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
        std::shared_ptr<PrenoteMessage> message3;
        std::shared_ptr<PrenoteMessage> message4;
        PrenoteMessageTimeout timeout;
    };

    TEST_F(PrenoteMessageTimeoutTest, ItPreservesNonAcknowledgedNonExpiredMessages)
    {
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message1))).Times(0);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message2))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message3))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message4))).Times(1);

        timeout.TimedEventTrigger();
        EXPECT_EQ(message1, messages->GetById(1));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItRemovesNonAcknowledgedExpiredMessages)
    {
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message2))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message3))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message4))).Times(1);

        timeout.TimedEventTrigger();
        EXPECT_EQ(nullptr, messages->GetById(2));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItPreservesMessagesAcknowledgedInThePastTenMinutes)
    {
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message1))).Times(0);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message2))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message3))).Times(0);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message4))).Times(1);

        auto timeBefore = TimeNow();
        SetTestNow(TimeNow() - std::chrono::minutes(9));
        message3->Acknowledge();
        SetTestNow(timeBefore);
        timeout.TimedEventTrigger();
        EXPECT_EQ(message3, messages->GetById(3));
    }

    TEST_F(PrenoteMessageTimeoutTest, ItRemovesMessagesAcknowledgedMoreThanTenMinutesAgo)
    {
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message2))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message3))).Times(1);
        EXPECT_CALL(*mockHandler, MessageTimeout(testing::Ref(*message4))).Times(1);

        auto timeBefore = TimeNow();
        SetTestNow(TimeNow() - std::chrono::minutes(11));
        message4->Acknowledge();
        SetTestNow(timeBefore);
        timeout.TimedEventTrigger();
        EXPECT_EQ(nullptr, messages->GetById(4));
    }
} // namespace UKControllerPluginTest::Prenote
