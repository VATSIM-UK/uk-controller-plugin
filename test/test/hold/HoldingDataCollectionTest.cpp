#include "pch/pch.h"
#include "hold//HoldingData.h"
#include "hold/HoldingDataCollection.h"

using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::HoldingDataCollection;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldingDataCollectionTest : public Test
        {
            public:
                HoldingData hold1 = { 1, "TIMBA", "TIMBA", 7000, 15000, 309, 1 };
                HoldingData hold2 = { 2, "WILLO", "WILLO", 7000, 15000, 285, 0 };
                HoldingDataCollection collection;
        };

        TEST_F(HoldingDataCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, collection.Count());
        }

        TEST_F(HoldingDataCollectionTest, ItAddsAHold)
        {
            collection.Add(hold1);
            collection.Add(hold2);
            EXPECT_EQ(2, collection.Count());
        }

        TEST_F(HoldingDataCollectionTest, ItDoesntAddDuplicates)
        {
            collection.Add(hold1);
            collection.Add(hold1);
            EXPECT_EQ(1, collection.Count());
        }

        TEST_F(HoldingDataCollectionTest, ItReturnsNotFoundIfHoldNotFound)
        {
            collection.Add(hold1);
            EXPECT_EQ(collection.notFound, collection.Get(2));
        }

        TEST_F(HoldingDataCollectionTest, TestItReturnsAHold)
        {
            collection.Add(hold1);;
            EXPECT_EQ(hold1, collection.Get(1));
        }

    }  // namespace Hold
}  // namespace UKControllerPluginTest
