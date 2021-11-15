#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"

using ::testing::ElementsAre;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Airfield::AirfieldValid;
using UKControllerPlugin::Airfield::CreateAirfieldCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Controller::ControllerPositionHierarchyFactory;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Airfield {

    class AirfieldCollectionFactoryTest : public Test
    {
        public:
        AirfieldCollectionFactoryTest() : factory(controllers)
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

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsValid)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        };

        EXPECT_TRUE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdMissing)
    {
        nlohmann::json airfield{
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdNotString)
    {
        nlohmann::json airfield{
            {"id", "abc"},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdentifierMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdentifierNotString)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", 123},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownNotArray)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", 1},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownEmpty)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array()},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownControllersInvalid)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, "abc"})},
        };

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfAirfieldNotObject)
    {
        EXPECT_FALSE(AirfieldValid(nlohmann::json::array(), factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, ItReturnsCollection)
    {
        auto airfields = nlohmann::json::array();
        airfields.push_back(nlohmann::json{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        });

        airfields.push_back(nlohmann::json{
            {"id", 2},
            {"identifier", "EGKK"},
            {"top_down_controller_positions", nlohmann::json::array({1})},
        });

        const auto collection = CreateAirfieldCollection(airfields, factory);
        EXPECT_NE(nullptr, collection);
        EXPECT_EQ(2, collection->GetSize());
        const auto airfieldOne = collection->FetchAirfieldByIcao("EGLL");
        EXPECT_NE(nullptr, airfieldOne);
        EXPECT_EQ(1, airfieldOne->Id());
        EXPECT_EQ("EGLL", airfieldOne->Icao());
        EXPECT_EQ(2, airfieldOne->TopDownOrder().CountPositions());

        const auto airfieldTwo = collection->FetchAirfieldByIcao("EGKK");
        EXPECT_NE(nullptr, airfieldTwo);
        EXPECT_EQ(2, airfieldTwo->Id());
        EXPECT_EQ("EGKK", airfieldTwo->Icao());
        EXPECT_EQ(1, airfieldTwo->TopDownOrder().CountPositions());
    }

    TEST_F(AirfieldCollectionFactoryTest, ItIgnoresBadAirfields)
    {
        auto airfields = nlohmann::json::array();
        airfields.push_back(nlohmann::json{
            {"id", "abc"}, // Bad
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
        });

        airfields.push_back(nlohmann::json{
            {"id", 2},
            {"identifier", 123}, // Bad
            {"top_down_controller_positions", nlohmann::json::array({1})},
        });

        const auto collection = CreateAirfieldCollection(airfields, factory);
        EXPECT_NE(nullptr, collection);
        EXPECT_EQ(0, collection->GetSize());
    }

    TEST_F(AirfieldCollectionFactoryTest, ItReturnsEmptyCollectionIfAirfieldsNotArray)
    {
        const auto collection = CreateAirfieldCollection(nlohmann::json::object(), factory);
        EXPECT_NE(nullptr, collection);
        EXPECT_EQ(0, collection->GetSize());
    }
} // namespace UKControllerPluginTest::Airfield
