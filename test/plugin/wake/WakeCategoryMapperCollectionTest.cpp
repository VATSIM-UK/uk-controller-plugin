#include "wake/WakeCategoryMapperCollection.h"

using UKControllerPlugin::Wake::WakeCategoryMapperCollection;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryMapperCollectionTest : public testing::Test
    {
        public:
        WakeCategoryMapperCollectionTest()
        {
            mapper1 = std::make_shared<testing::NiceMock<Wake::MockWakeCategoryMapper>>();
            mapper2 = std::make_shared<testing::NiceMock<Wake::MockWakeCategoryMapper>>();
        }

        std::shared_ptr<testing::NiceMock<Wake::MockWakeCategoryMapper>> mapper1;
        std::shared_ptr<testing::NiceMock<Wake::MockWakeCategoryMapper>> mapper2;
        WakeCategoryMapperCollection collection;
    };

    TEST_F(WakeCategoryMapperCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(WakeCategoryMapperCollectionTest, ItAddsMappers)
    {
        collection.Add(1, mapper1);
        collection.Add(2, mapper2);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(WakeCategoryMapperCollectionTest, ItDoesntAddDuplicateMappers)
    {
        collection.Add(1, mapper1);
        collection.Add(1, mapper1);
        collection.Add(1, mapper1);
        collection.Add(2, mapper1);
        collection.Add(2, mapper1);
        collection.Add(2, mapper1);
        EXPECT_EQ(2, collection.Count());
    }

    TEST_F(WakeCategoryMapperCollectionTest, ItReturnsMappers)
    {
        collection.Add(1, mapper1);
        collection.Add(2, mapper2);
        EXPECT_EQ(mapper2, collection.Get(2));
    }

    TEST_F(WakeCategoryMapperCollectionTest, ItReturnsNullptrMapperNotFound)
    {
        collection.Add(1, mapper1);
        collection.Add(2, mapper2);
        EXPECT_EQ(nullptr, collection.Get(3));
    }
} // namespace UKControllerPluginTest::Wake
