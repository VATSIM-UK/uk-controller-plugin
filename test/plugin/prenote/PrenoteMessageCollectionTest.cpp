#include "prenote/PrenoteMessageCollection.h"
#include "prenote/PrenoteMessage.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Prenote::PrenoteMessage;
using UKControllerPlugin::Prenote::PrenoteMessageCollection;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Prenote {
    class PrenoteMessageCollectionTest : public ::testing::Test
    {
        public:
        PrenoteMessageCollectionTest()
        {
            message1 = std::make_shared<PrenoteMessage>(1, "BAW123", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow());
            message2 = std::make_shared<PrenoteMessage>(5, "BAW456", "EGGD", "BADIM1X", "EGLL", 1, 2, TimeNow());
        }

        PrenoteMessageCollection collection;
        std::shared_ptr<PrenoteMessage> message1;
        std::shared_ptr<PrenoteMessage> message2;
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
        auto iter = collection.cbegin();
        EXPECT_EQ(message1, *iter++);
        EXPECT_EQ(message2, *iter++);
        EXPECT_EQ(collection.cend(), iter);
    }
} // namespace UKControllerPluginTest::Prenote
