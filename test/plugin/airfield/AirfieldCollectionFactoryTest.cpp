#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPositionHierarchyFactory.h"
#include "controller/ControllerPositionHierarchy.h"
#include "prenote/PairedAirfieldPrenote.h"

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
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_TRUE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsValidWhenHandoffIdNull)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", nlohmann::json::value_t::null},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_TRUE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdMissing)
    {
        nlohmann::json airfield{
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdNotString)
    {
        nlohmann::json airfield{
            {"id", "abc"},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdentifierMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfIdentifierNotString)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", 123},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownNotArray)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", 1},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfTopDownControllersInvalid)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, "abc"})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenoteAirfieldIdMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"handoff_id", 1},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"pairing_prenotes", nlohmann::json::array({{{"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenoteAirfieldIdNotInteger)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", "abc"}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenoteFlightRuleIdMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes", nlohmann::json::array({{{"airfield_id", 1}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenoteFlightRuleIdNotInteger)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", "abc"}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenotePrenoteIdMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes", nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenotePrenoteIdNotInteger)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", "abc"}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenotePrenoteNotObject)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes", nlohmann::json::array({nlohmann::json::array()})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfPrenotesNotArray)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", 1},
            {"pairing_prenotes", nlohmann::json::object()}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfHandoffIdMissing)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

        EXPECT_FALSE(AirfieldValid(airfield, factory));
    }

    TEST_F(AirfieldCollectionFactoryTest, AirfieldIsNotValidIfHandoffIdInvalid)
    {
        nlohmann::json airfield{
            {"id", 1},
            {"identifier", "EGLL"},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"handoff_id", "abc"},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 1}}})}};

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
            {"handoff_id", 1},
            {"pairing_prenotes",
             nlohmann::json::array({{{"airfield_id", 1}, {"flight_rule_id", 2}, {"prenote_id", 3}}})}});

        airfields.push_back(nlohmann::json{
            {"id", 2},
            {"identifier", "EGKK"},
            {"top_down_controller_positions", nlohmann::json::array({})},
            {"handoff_id", nlohmann::json::value_t::null},
            {"pairing_prenotes", nlohmann::json::array()}});

        const auto collection = CreateAirfieldCollection(airfields, factory);
        EXPECT_NE(nullptr, collection);
        EXPECT_EQ(2, collection->GetSize());
        const auto airfieldOne = collection->FetchAirfieldByIcao("EGLL");
        EXPECT_NE(nullptr, airfieldOne);
        EXPECT_EQ(1, airfieldOne->Id());
        EXPECT_EQ("EGLL", airfieldOne->Icao());
        EXPECT_EQ(1, airfieldOne->DefaultHandoff());
        EXPECT_EQ(2, airfieldOne->TopDownOrder().CountPositions());
        EXPECT_EQ(1, airfieldOne->AirfieldPairingPrenotes().size());
        EXPECT_EQ(1, (*airfieldOne->AirfieldPairingPrenotes().cbegin())->targetAirfield);
        EXPECT_EQ(2, (*airfieldOne->AirfieldPairingPrenotes().cbegin())->flightRuleId);
        EXPECT_EQ(3, (*airfieldOne->AirfieldPairingPrenotes().cbegin())->prenoteId);

        const auto airfieldTwo = collection->FetchAirfieldByIcao("EGKK");
        EXPECT_NE(nullptr, airfieldTwo);
        EXPECT_EQ(2, airfieldTwo->Id());
        EXPECT_EQ("EGKK", airfieldTwo->Icao());
        EXPECT_FALSE(airfieldTwo->HasDefaultHandoff());
        EXPECT_EQ(0, airfieldTwo->TopDownOrder().CountPositions());
        EXPECT_EQ(0, airfieldTwo->AirfieldPairingPrenotes().size());
    }

    TEST_F(AirfieldCollectionFactoryTest, ItIgnoresBadAirfields)
    {
        auto airfields = nlohmann::json::array();
        airfields.push_back(nlohmann::json{
            {"id", "abc"}, // Bad
            {"identifier", "EGLL"},
            {"handoff_id", 1},
            {"top_down_controller_positions", nlohmann::json::array({1, 2})},
            {"pairing_prenotes", nlohmann::json::array()}});

        airfields.push_back(nlohmann::json{
            {"id", 2},
            {"identifier", 123}, // Bad
            {"handoff_id", 1},
            {"top_down_controller_positions", nlohmann::json::array({1})},
            {"pairing_prenotes", nlohmann::json::array()}});

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
