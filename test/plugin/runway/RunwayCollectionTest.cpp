#include "runway/Runway.h"
#include "runway/RunwayCollection.h"

using UKControllerPlugin::Runway::Runway;
using UKControllerPlugin::Runway::RunwayCollection;

namespace UKControllerPluginTest::Runway {
    class RunwayCollectionTest : public testing::Test
    {
        public:
        RunwayCollectionTest()
            : runway1(std::make_shared<class Runway>(1, 2, "05", 50, GetPosition())),
              runway2(std::make_shared<class Runway>(2, 2, "27L", 270, GetPosition())),
              runway3(std::make_shared<class Runway>(3, 3, "27L", 275, GetPosition()))
        {
        }

        std::shared_ptr<class Runway> runway1;
        std::shared_ptr<class Runway> runway2;
        std::shared_ptr<class Runway> runway3;
        RunwayCollection collection;

        private:
        auto GetPosition() -> EuroScopePlugIn::CPosition
        {
            EuroScopePlugIn::CPosition position;
            position.m_Latitude = 3;
            position.m_Longitude = 4;
            return position;
        }
    };

    TEST_F(RunwayCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(RunwayCollectionTest, ItAddsRunways)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);
        EXPECT_EQ(3, collection.Count());
    }

    TEST_F(RunwayCollectionTest, ItDoesntAddDuplicateRunways)
    {
        collection.Add(runway1);
        collection.Add(runway1);
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway2);
        collection.Add(runway2);
        collection.Add(runway3);
        collection.Add(runway3);
        collection.Add(runway3);
        EXPECT_EQ(3, collection.Count());
    }

    TEST_F(RunwayCollectionTest, ItFetchesARunwayById)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(runway2, collection.GetById(2));
    }

    TEST_F(RunwayCollectionTest, ItReturnsNullptrIfInvalidId)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(nullptr, collection.GetById(5));
    }

    TEST_F(RunwayCollectionTest, ItFetchesARunwayByIdentifierAndAirfild)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(runway1, collection.GetByAirfieldAndIdentifier(2, "05"));
    }

    TEST_F(RunwayCollectionTest, ItReturnsNullptrIfAirfieldIdDoesntMatch)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(nullptr, collection.GetByAirfieldAndIdentifier(3, "05"));
    }

    TEST_F(RunwayCollectionTest, ItReturnsNullptrIfIdentifierDoesntMatch)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(nullptr, collection.GetByAirfieldAndIdentifier(2, "27R"));
    }

    TEST_F(RunwayCollectionTest, ItFetchesARunwayByIdentifierAndAirfildIdentifier)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(runway1, collection.GetByAirfieldAndIdentifier("EGKK", "05"));
    }

    TEST_F(RunwayCollectionTest, ItReturnsNullptrIfAirfieldIdentifierDoesntMatch)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(nullptr, collection.GetByAirfieldAndIdentifier("EGLL", "05"));
    }

    TEST_F(RunwayCollectionTest, ItReturnsNullptrIfIdentifierDoesntMatchWhenUsingAirfieldIdentifier)
    {
        collection.Add(runway1);
        collection.Add(runway2);
        collection.Add(runway3);

        EXPECT_EQ(nullptr, collection.GetByAirfieldAndIdentifier("EGKK", "27R"));
    }
} // namespace UKControllerPluginTest::Runway
