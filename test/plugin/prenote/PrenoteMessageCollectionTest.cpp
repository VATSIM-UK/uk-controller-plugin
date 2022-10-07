#include "controller/ControllerPosition.h"
#include "prenote/PrenoteMessage.h"
#include "prenote/PrenoteMessageCollection.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageCollectionTest : public ::testing::Test
    {
        public:
        PrenoteMessageCollectionTest()
        {
            sendingPosition = std::make_shared<ControllerPosition>(
                1, "EGKK_TWR", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            receivingPosition = std::make_shared<ControllerPosition>(
                2, "EGKK_F_APP", 124.225, std::vector<std::string>{"EGKK"}, true, false);
            message1 = std::make_shared<PrenoteMessage>(
                1, "BAW123", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
            message2 = std::make_shared<PrenoteMessage>(
                5, "BAW456", "EGGD", "BADIM1X", "EGKK", sendingPosition, receivingPosition, TimeNow());
            message3 = std::make_shared<PrenoteMessage>(
                3, "BAW456", "EGGD", "BADIM1X", "EGLL", sendingPosition, receivingPosition, TimeNow());
        }

        std::shared_ptr<ControllerPosition> sendingPosition;
        std::shared_ptr<ControllerPosition> receivingPosition;
        PrenoteMessageCollection collection;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
        std::shared_ptr<PrenoteMessage> message3;
    };

    TEST_F(PrenoteMessageCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(PrenoteMessageCollectionTest, ItAddsMessages)
    {
        collection.Add(message1);
        collection.Add(message2);
        EXPECT_EQ(2, collection.Count());
        EXPECT_EQ(message1, collection.GetById(1));
        EXPECT_EQ(message2, collection.GetById(5));
    }

    TEST_F(PrenoteMessageCollectionTest, ItDoesntAddDuplicateMessages)
    {
        collection.Add(message1);
        collection.Add(message1);
        collection.Add(message1);
        collection.Add(message2);
        collection.Add(message2);
        collection.Add(message2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(PrenoteMessageCollectionTest, ItReturnsNullptrIfNoMessageFound)
    {
        collection.Add(message1);
        collection.Add(message2);
        EXPECT_EQ(nullptr, collection.GetById(999));
    }

    TEST_F(PrenoteMessageCollectionTest, ItRemovesMessages)
    {
        collection.Add(message1);
        collection.Add(message2);
        collection.Remove(1);
        collection.Remove(5);
        EXPECT_EQ(0, collection.Count());
        EXPECT_EQ(nullptr, collection.GetById(1));
        EXPECT_EQ(nullptr, collection.GetById(5));
    }

    TEST_F(PrenoteMessageCollectionTest, ItHandlesRemovingNonExistentMessages)
    {
        collection.Add(message1);
        collection.Remove(5);
    }

    TEST_F(PrenoteMessageCollectionTest, ItIsIterable)
    {
        collection.Add(message1);
        collection.Add(message2);
        collection.Iterate([](const std::shared_ptr<PrenoteMessage>& message) { message->Acknowledge(); });
        EXPECT_TRUE(message1->IsAcknowledged());
        EXPECT_TRUE(message2->IsAcknowledged());
    }

    TEST_F(PrenoteMessageCollectionTest, ItReturnsTheFirstMessageMatchingAPredicate)
    {
        collection.Add(message1);
        collection.Add(message2);
        EXPECT_EQ(message2, collection.FirstWhere([](const std::shared_ptr<PrenoteMessage>& message) -> bool {
            return message->GetCallsign() == "BAW456";
        }));
    }

    TEST_F(PrenoteMessageCollectionTest, ItReturnsAnyMessageMatchingAPredicate)
    {
        collection.Add(message1);
        collection.Add(message2);
        auto returnedMessage = collection.FirstWhere(
            [](const std::shared_ptr<PrenoteMessage>& message) -> bool { return message->GetSid() == "BADIM1X"; });
        EXPECT_TRUE(returnedMessage == message1 || returnedMessage == message2);
    }

    TEST_F(PrenoteMessageCollectionTest, ItReturnsNullptrIfNothingMatchesPredicate)
    {
        collection.Add(message1);
        collection.Add(message2);
        EXPECT_EQ(nullptr, collection.FirstWhere([](const std::shared_ptr<PrenoteMessage>& message) -> bool {
            return message->GetCallsign() == "BAW99";
        }));
    }

    TEST_F(PrenoteMessageCollectionTest, ItemsCanBeRemovedByPredicate)
    {
        collection.Add(message1);
        collection.Add(message2);
        collection.RemoveWhere([](const std::shared_ptr<PrenoteMessage>& message) { return message->GetId() == 1; });
        EXPECT_EQ(1, collection.Count());
        EXPECT_EQ(message2, collection.GetById(5));
        EXPECT_EQ(nullptr, collection.GetById(1));
        collection.RemoveWhere([](const std::shared_ptr<PrenoteMessage>& message) { return message->GetId() == 5; });
        EXPECT_EQ(0, collection.Count());
        EXPECT_EQ(nullptr, collection.GetById(5));
    }

    TEST_F(PrenoteMessageCollectionTest, ItemsRemovedByPredicateArePassedToCallback)
    {
        std::vector<int> prenoteIds;
        collection.Add(message1);
        collection.Add(message2);
        collection.Add(message3);
        collection.RemoveWhere(
            [](const std::shared_ptr<PrenoteMessage>& message) { return message->GetDestinationAirfield() == "EGLL"; },
            [&prenoteIds](const PrenoteMessage& message) { prenoteIds.push_back(message.GetId()); });

        EXPECT_EQ(1, collection.Count());
        EXPECT_EQ(std::vector<int>({1, 3}), prenoteIds);
    }
} // namespace UKControllerPluginTest::Prenote
