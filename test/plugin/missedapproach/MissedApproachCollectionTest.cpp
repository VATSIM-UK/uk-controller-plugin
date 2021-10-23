#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;

namespace UKControllerPluginTest::MissedApproach {
    class MissedApproachCollectionTest : public testing::Test
    {
        public:
        MissedApproachCollectionTest()
            : missed1(std::make_shared<class MissedApproach>(1, "BAW123", std::chrono::system_clock::now(), true)),
              missed2(std::make_shared<class MissedApproach>(2, "BAW456", std::chrono::system_clock::now(), true))
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

    TEST_F(MissedApproachCollectionTest, ItReturnsApproachByCallsign)
    {
        collection.Add(missed1);
        EXPECT_EQ(missed1, collection.Get("BAW123"));
    }

    TEST_F(MissedApproachCollectionTest, ItReturnsNullptrOnNonExistentApproachByCallsign)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        EXPECT_EQ(nullptr, collection.Get("BAW999"));
    }

    TEST_F(MissedApproachCollectionTest, ItReturnsApproachById)
    {
        collection.Add(missed1);
        EXPECT_EQ(missed1, collection.Get(1));
    }

    TEST_F(MissedApproachCollectionTest, ItReturnsNullptrOnNonExistentApproachById)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        EXPECT_EQ(nullptr, collection.Get(999));
    }

    TEST_F(MissedApproachCollectionTest, ItHandlesNoRemovalsNothingToRemove)
    {
        EXPECT_NO_THROW(
            collection.RemoveWhere([](const std::shared_ptr<class MissedApproach>&) -> bool { return false; }));
    }

    TEST_F(MissedApproachCollectionTest, ItHandlesNoRemovals)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        EXPECT_NO_THROW(
            collection.RemoveWhere([](const std::shared_ptr<class MissedApproach>&) -> bool { return false; }));
    }

    TEST_F(MissedApproachCollectionTest, ItRemovesBasedOnPredicate)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        collection.RemoveWhere(
            [](const std::shared_ptr<class MissedApproach>& app) -> bool { return app->Callsign() == "BAW123"; });
        EXPECT_EQ(1, collection.Count());
        EXPECT_EQ(nullptr, collection.Get("BAW123"));
        EXPECT_EQ(missed2, collection.Get("BAW456"));
    }

    TEST_F(MissedApproachCollectionTest, FirstWhereReturnsItem)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        auto result = collection.FirstWhere(
            [](const std::shared_ptr<class MissedApproach>& app) -> bool { return app->Callsign() == "BAW123"; });
        EXPECT_EQ(missed1, result);
    }

    TEST_F(MissedApproachCollectionTest, FirstWhereReturnsNullptrIfNotFound)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        auto result = collection.FirstWhere(
            [](const std::shared_ptr<class MissedApproach>& app) -> bool { return app->Callsign() == "BAW999"; });
        EXPECT_EQ(nullptr, result);
    }

    TEST_F(MissedApproachCollectionTest, ItRemovesBasedOnValue)
    {
        collection.Add(missed1);
        collection.Add(missed2);
        collection.Remove(missed1);
        EXPECT_EQ(1, collection.Count());
        EXPECT_EQ(nullptr, collection.Get("BAW123"));
        EXPECT_EQ(missed2, collection.Get("BAW456"));
    }

    TEST_F(MissedApproachCollectionTest, ItHandlesRemoveWhenNoValue)
    {
        collection.Remove(missed1);
        EXPECT_NO_THROW(collection.Remove(missed1));
    }

    TEST_F(MissedApproachCollectionTest, ItIteratesTheCollection)
    {
        collection.Add(missed1);
        collection.Add(missed2);

        std::vector<std::string> expected({"BAW123", "BAW456"});
        std::vector<std::string> actual;
        collection.ForEach(
            [&actual](const std::shared_ptr<class MissedApproach>& missed) { actual.push_back(missed->Callsign()); });

        EXPECT_EQ(expected, actual);
    }
} // namespace UKControllerPluginTest::MissedApproach
