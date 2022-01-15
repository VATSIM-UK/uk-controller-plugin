#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldingData.h"
#include "hold/PublishedHoldCollection.h"

using ::testing::Test;
using UKControllerPlugin::Hold::CompareHolds;
using UKControllerPlugin::Hold::HoldingData;
using UKControllerPlugin::Hold::PublishedHoldCollection;

namespace UKControllerPluginTest {
    namespace Hold {

        class PublishedHoldCollectionTest : public Test
        {
            public:
            HoldingData hold1 = {1, "TIMBA", "TIMBA", 8000, 15000, 209, "left"};
            HoldingData hold1Copy = {1, "TIMBA", "TIMBA", 8000, 15000, 209, "left"};
            HoldingData hold2 = {2, "WILLO", "WILLO", 8000, 15000, 209, "left"};
            HoldingData hold3 = {3, "WILLO", "WILLO", 8000, 9000, 209, "right"};
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

            const std::set<const HoldingData*> expected = {};
            EXPECT_EQ(expected, this->collection.GetForFix("MAY"));
        }

        TEST_F(PublishedHoldCollectionTest, GetReturnsHoldsForAFix)
        {
            this->collection.Add(std::move(this->hold1));
            this->collection.Add(std::move(this->hold2));
            this->collection.Add(std::move(this->hold3));

            std::set<const HoldingData*> holds = this->collection.GetForFix("WILLO");

            EXPECT_EQ(2, holds.size());
            EXPECT_EQ(2, std::count_if(holds.cbegin(), holds.cend(), [this](auto hold) -> bool {
                          return (*hold) == this->hold2 || (*hold) == this->hold3;
                      }));
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
    } // namespace Hold
} // namespace UKControllerPluginTest
