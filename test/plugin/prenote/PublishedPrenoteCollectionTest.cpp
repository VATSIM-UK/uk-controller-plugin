#include "controller/ControllerPositionHierarchy.h"
#include "prenote/PublishedPrenote.h"
#include "prenote/PublishedPrenoteCollection.h"

using UKControllerPlugin::Prenote::PublishedPrenote;
using UKControllerPlugin::Prenote::PublishedPrenoteCollection;

namespace UKControllerPluginTest::Prenote {
    class PublishedPrenoteCollectionTest : public testing::Test
    {
        public:
        PublishedPrenoteCollectionTest()
        {
            this->prenote1 = std::make_shared<PublishedPrenote>(1, "One", nullptr);
            this->prenote2 = std::make_shared<PublishedPrenote>(2, "Two", nullptr);
        }

        std::shared_ptr<PublishedPrenote> prenote1;
        std::shared_ptr<PublishedPrenote> prenote2;
        PublishedPrenoteCollection collection;
    };

    TEST_F(PublishedPrenoteCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(PublishedPrenoteCollectionTest, ItAddsPrenotes)
    {
        collection.Add(prenote1);
        collection.Add(prenote2);

        EXPECT_EQ(2, collection.Count());
        EXPECT_EQ(prenote1, collection.Get(1));
        EXPECT_EQ(prenote2, collection.Get(2));
    }

    TEST_F(PublishedPrenoteCollectionTest, ItDoesntAddDuplicatePrenotes)
    {
        collection.Add(prenote1);
        collection.Add(prenote1);
        collection.Add(prenote1);
        collection.Add(prenote2);
        collection.Add(prenote2);
        collection.Add(prenote2);

        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(PublishedPrenoteCollectionTest, ItReturnsNullptrIfPrenoteNotFound)
    {
        collection.Add(prenote1);
        collection.Add(prenote2);

        EXPECT_EQ(nullptr, collection.Get(3));
    }
} // namespace UKControllerPluginTest::Prenote
