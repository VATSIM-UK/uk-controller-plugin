#include "collection/Collection.h"

namespace UKControllerPluginUtilsTest::Collection {

    struct MockCollectionValue
    {
        MockCollectionValue(int id) : id(id)
        {
        }

        [[nodiscard]] auto CollectionKey() const -> int
        {
            return id;
        }

        int id;
    };

    class CollectionTest : public ::testing::Test
    {
        public:
        UKControllerPluginUtils::Collection::Collection<int, MockCollectionValue> collection;
    };

    TEST_F(CollectionTest, ItAddsItems)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_EQ(1, collection.Count());
    }

    TEST_F(CollectionTest, ItDoesntAddDuplicateItems)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.Add(item);
        EXPECT_EQ(1, collection.Count());
    }

    TEST_F(CollectionTest, ItGetsItemsByKey)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_EQ(item, collection.Get(1));
    }

    TEST_F(CollectionTest, ItReturnsNullptrIfNoItemFoundByKey)
    {
        EXPECT_EQ(nullptr, collection.Get(1));
    }

    TEST_F(CollectionTest, ItGetsItemsByItem)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_EQ(item, collection.Get(item));
    }

    TEST_F(CollectionTest, ItReturnsNullptrIfNoItemFoundByItem)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        EXPECT_EQ(nullptr, collection.Get(item));
    }

    TEST_F(CollectionTest, ItContainsItemsByKey)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_TRUE(collection.Contains(1));
    }

    TEST_F(CollectionTest, ItDoesNotContainItemsByKey)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_FALSE(collection.Contains(2));
    }

    TEST_F(CollectionTest, ItContainsItemsByItem)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        EXPECT_TRUE(collection.Contains(item));
    }

    TEST_F(CollectionTest, ItDoesNotContainItemsByItem)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        auto item2 = std::make_shared<MockCollectionValue>(2);
        collection.Add(item);
        EXPECT_FALSE(collection.Contains(item2));
    }

    TEST_F(CollectionTest, ItRemovesItemsByKey)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.RemoveByKey(1);
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(CollectionTest, ItRemovesItemsByItem)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.Remove(item);
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(CollectionTest, ItDoesNotRemoveItemsByKeyIfNotPresent)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.RemoveByKey(2);
        EXPECT_EQ(1, collection.Count());
    }

    TEST_F(CollectionTest, ItDoesNotRemoveItemsByItemIfNotPresent)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.Remove(std::make_shared<MockCollectionValue>(2));
        EXPECT_EQ(1, collection.Count());
    }

    TEST_F(CollectionTest, ItDoesNotRemoveItemsByKeyIfNull)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.RemoveByKey(0);
        EXPECT_EQ(1, collection.Count());
    }

    TEST_F(CollectionTest, ItDoesNotRemoveItemsByItemIfNull)
    {
        auto item = std::make_shared<MockCollectionValue>(1);
        collection.Add(item);
        collection.Remove(nullptr);
        EXPECT_EQ(1, collection.Count());
    }
} // namespace UKControllerPluginUtilsTest::Collection
