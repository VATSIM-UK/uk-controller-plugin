#include "pch/pch.h"
#include "hold/PublishedHoldCollection.h"
#include "hold/HoldingData.h"

using ::testing::Test;
using UKControllerPlugin::Hold::PublishedHoldCollection;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::CompareHolds;

namespace UKControllerPluginTest {
    namespace Hold {

        class PublishedHoldCollectionTest : public Test
        {
            public:
                HoldingData hold1 = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
                HoldingData hold1Copy = { 1, "TIMBA", "TIMBA", 8000, 15000, 209, "left" };
                HoldingData hold2 = { 2, "WILLO", "WILLO", 8000, 15000, 209, "left" };
                HoldingData hold3 = { 3, "WILLO", "WILLO", 8000, 9000, 209, "right" };
                PublishedHoldCollection collection;
        };

        TEST_F(PublishedHoldCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, this->collection.Count());
        }

        TEST_F(PublishedHoldCollectionTest, ItAddsHoldingData)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));
            EXPECT_EQ(3, this->collection.Count());
        }

        TEST_F(PublishedHoldCollectionTest, ItDoesntAddDuplicateHoldingData)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold1Copy));
            EXPECT_EQ(1, this->collection.Count());
        }

        TEST_F(PublishedHoldCollectionTest, GetReturnsEmptyIfNoHolds)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));

            const std::set<HoldingData, CompareHolds> expected = {};
            EXPECT_EQ(expected, this->collection.Get("MAY"));
        }

        TEST_F(PublishedHoldCollectionTest, GetReturnsHoldsForAFix)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));

            std::set<HoldingData, CompareHolds> expected;
            expected.emplace(std::move(hold2));
            expected.emplace(std::move(hold3));
            EXPECT_EQ(expected, this->collection.Get("WILLO"));
        }

        TEST_F(PublishedHoldCollectionTest, GetByIdReturnsHoldById)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));

            EXPECT_EQ(this->hold1, this->collection.GetById(1));
        }

        TEST_F(PublishedHoldCollectionTest, GetByIdReturnsNoHoldIfDoesntExist)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));

            EXPECT_EQ(this->collection.noHold, this->collection.GetById(999));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
