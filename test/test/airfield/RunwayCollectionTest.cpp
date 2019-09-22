#pragma once
#include "pch/pch.h"
#include "airfield/Runway.h"
#include "airfield/RunwayCollection.h"

using ::testing::Test;
using UKControllerPlugin::Airfield::Runway;
using UKControllerPlugin::Airfield::RunwayCollection;

namespace UKControllerPluginTest {
    namespace Airfield {

        class RunwayCollectionTest : public Test
        {
            public:

                RunwayCollectionTest()
                {
                    runway1.reset(new Runway("sfi", "EGKK", "26L", 261));
                    runway2.reset(new Runway("sfi2", "EGKK", "08R", 79));
                }

                std::shared_ptr<Runway> runway1;
                std::shared_ptr<Runway> runway2;
                RunwayCollection collection;
        };

        TEST_F(RunwayCollectionTest, ItStartsEmpty)
        {
            EXPECT_EQ(0, collection.Count());
        }

        TEST_F(RunwayCollectionTest, ItAddsRunways)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway2);
            EXPECT_EQ(2, collection.Count());
        }

        TEST_F(RunwayCollectionTest, ItDoesntAddDuplicateRunways)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway1);
            EXPECT_EQ(1, collection.Count());
        }

        TEST_F(RunwayCollectionTest, ItReturnsInvalidIfNotFoundByAirfield)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway2);
            EXPECT_EQ(this->collection.invalidRunway, this->collection.FetchByIdentifierAndAirfield("27L", "EGKK"));
        }

        TEST_F(RunwayCollectionTest, ItReturnsRunwayIfFound)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway2);
            EXPECT_EQ(*this->runway1, this->collection.FetchByIdentifierAndAirfield("26L", "EGKK"));
        }

        TEST_F(RunwayCollectionTest, ItReturnsNullPtrIfNotFound)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway2);
            EXPECT_EQ(nullptr, this->collection.FetchBySectorFileIdentifier("sfix"));
        }

        TEST_F(RunwayCollectionTest, ItReturnsPointerIfFound)
        {
            this->collection.AddRunway(runway1);
            this->collection.AddRunway(runway2);
            EXPECT_EQ(this->runway1, this->collection.FetchBySectorFileIdentifier("sfi"));
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest