#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageEventHandlerCollection.h"

using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageEventHandlerCollection;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageEventHandlerCollectionTest : public testing::Test
    {
        public:
        PrenoteMessageEventHandlerCollection collection;
    };

    TEST_F(PrenoteMessageEventHandlerCollectionTest, ItStartsWithNoHandlers)
    {
        EXPECT_EQ(0, collection.CountHandlers());
    }

    TEST_F(PrenoteMessageEventHandlerCollectionTest, ItAddsHandlers)
    {
        auto handler1 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        auto handler2 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);
        EXPECT_EQ(2, collection.CountHandlers());
    }

    TEST_F(PrenoteMessageEventHandlerCollectionTest, ItProcessesNewMessageEvent)
    {
        auto handler1 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        auto handler2 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);

        const PrenoteMessage message(
            1, "BAW123", "EGKK", "TEST1A", "EGLL", nullptr, nullptr, std::chrono::system_clock::now());

        EXPECT_CALL(*handler1, NewMessage(testing::Ref(message))).Times(1);

        EXPECT_CALL(*handler2, NewMessage(testing::Ref(message))).Times(1);

        collection.NewMessage(message);
    }

    TEST_F(PrenoteMessageEventHandlerCollectionTest, ItProcessesCancelledMessageEvent)
    {
        auto handler1 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        auto handler2 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);

        const PrenoteMessage message(
            1, "BAW123", "EGKK", "TEST1A", "EGLL", nullptr, nullptr, std::chrono::system_clock::now());

        EXPECT_CALL(*handler1, MessageCancelled(testing::Ref(message))).Times(1);

        EXPECT_CALL(*handler2, MessageCancelled(testing::Ref(message))).Times(1);

        collection.MessageCancelled(message);
    }

    TEST_F(PrenoteMessageEventHandlerCollectionTest, ItProcessesAcknowledgedMessageEvent)
    {
        auto handler1 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        auto handler2 = std::make_shared<testing::NiceMock<MockPrenoteMessageEventHandlerInterface>>();
        collection.AddHandler(handler1);
        collection.AddHandler(handler2);

        const PrenoteMessage message(
            1, "BAW123", "EGKK", "TEST1A", "EGLL", nullptr, nullptr, std::chrono::system_clock::now());

        EXPECT_CALL(*handler1, MessageAcknowledged(testing::Ref(message))).Times(1);

        EXPECT_CALL(*handler2, MessageAcknowledged(testing::Ref(message))).Times(1);

        collection.MessageAcknowledged(message);
    }
} // namespace UKControllerPluginTest::Prenote
