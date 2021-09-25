#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachCollectionTest : public testing::Test
    {
        public:
        MissedApproachCollectionTest()
            : missed1(std::make_shared<class MissedApproach>("BAW123", std::chrono::system_clock::now())),
              missed2(std::make_shared<class MissedApproach>("BAW456", std::chrono::system_clock::now()))
        {
        }
        std::shared_ptr<class MissedApproach> missed1;
        std::shared_ptr<class MissedApproach> missed2;
        MissedApproachCollection collection;
    };

    TEST_F(MissedApproachCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(MissedApproachCollectionTest, ItAddsMissed)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        EXPECT_EQ(2, collection.Count());
        EXPECT_EQ(missed1, collection.Get("BAW123"));
        EXPECT_EQ(missed2, collection.Get("BAW456"));
    }

    TEST_F(MissedApproachCollectionTest, ItDoesntAddDuplicateMissed)
    {
        collection.Add(missed1);
        collection.Add(missed1);
        collection.Add(missed1);
        collection.Add(missed2);
        collection.Add(missed2);
        collection.Add(missed2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(MissedApproachCollectionTest, ItReturnsNullptrOnNonExistentApproach)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        EXPECT_EQ(nullptr, collection.Get("BAW999"));
    }
} // namespace UKControllerPluginTest::MissedApproach
