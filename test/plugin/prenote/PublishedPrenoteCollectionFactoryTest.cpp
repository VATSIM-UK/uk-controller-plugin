#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchy.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "prenote/PublishedPrenote.h"
#include "prenote/PublishedPrenoteCollection.h"
#include "prenote/PublishedPrenoteCollectionFactory.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPlugin::Prenote::CreatePublishedPrenoteCollection;
using UKControllerPlugin::Prenote::PrenoteValid;
using UKControllerPlugin::Prenote::PublishedPrenoteCollection;

namespace UKControllerPluginTest::Prenote {
    class PublishedPrenoteCollectionFactoryTest : public testing::Test
    {
        public:
        PublishedPrenoteCollectionFactoryTest() : factory(controllers)
        {
            std::vector<std::string> handoffs = {"EGKK"};
            controllers.AddPosition(
                std::make_shared<ControllerPosition>(1, "EGKK_APP", 126.820, handoffs, true, false));
            controllers.AddPosition(
                std::make_shared<ControllerPosition>(2, "LON_S_CTR", 129.420, handoffs, true, false));
        }

        ControllerPositionCollection controllers;
        ControllerPositionHierarchyFactory factory;
    };

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsValid)
    {
        nlohmann::json data = {{"id", 1}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_TRUE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfIdMissing)
    {
        nlohmann::json data = {{"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfIdNotInterger)
    {
        nlohmann::json data = {{"id", "abc"}, {"controller_positions", nlohmann::json::array({1, 2})}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfControllersMissing)
    {
        nlohmann::json data = {{"id", 1}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfControllersNotArray)
    {
        nlohmann::json data = {{"id", 1}, {"controller_positions", nlohmann::json::object()}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfControllersInvalid)
    {
        nlohmann::json data = {{"id", 1}, {"controller_positions", nlohmann::json::array({"ab", "cd"})}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfControllersEmpty)
    {
        nlohmann::json data = {{"id", 1}, {"controller_positions", nlohmann::json::array()}};

        EXPECT_FALSE(PrenoteValid(data, factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, APrenoteIsInvalidIfNotObject)
    {
        EXPECT_FALSE(PrenoteValid(nlohmann::json::array(), factory));
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, ItReturnsACollection)
    {
        auto data = nlohmann::json::array();
        data.push_back({{"id", 1}, {"controller_positions", nlohmann::json::array({1, 2})}});
        data.push_back({{"id", 2}, {"controller_positions", nlohmann::json::array({1})}});

        auto collection = CreatePublishedPrenoteCollection(data, factory);
        EXPECT_EQ(2, collection->Count());
        auto prenoteOne = collection->Get(1);
        EXPECT_EQ(1, prenoteOne->id);
        EXPECT_EQ(2, prenoteOne->controllers->CountPositions());

        auto prenoteTwo = collection->Get(2);
        EXPECT_EQ(2, prenoteTwo->id);
        EXPECT_EQ(1, prenoteTwo->controllers->CountPositions());
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, ItIgnoresInvalidPrenotes)
    {
        auto data = nlohmann::json::array();
        data.push_back({{"id", 1}, {"controller_positions", nlohmann::json::array({1, 2})}});
        data.push_back({{"id", "invalid"}, {"controller_positions", nlohmann::json::array({1})}});

        auto collection = CreatePublishedPrenoteCollection(data, factory);
        EXPECT_EQ(1, collection->Count());
        auto prenoteOne = collection->Get(1);
        EXPECT_EQ(1, prenoteOne->id);
        EXPECT_EQ(2, prenoteOne->controllers->CountPositions());
    }

    TEST_F(PublishedPrenoteCollectionFactoryTest, ItReturnsEmptyCollectionIfInvalidData)
    {
        auto data = nlohmann::json::object();
        auto collection = CreatePublishedPrenoteCollection(data, factory);
        EXPECT_EQ(0, collection->Count());
    }
} // namespace UKControllerPluginTest::Prenote
