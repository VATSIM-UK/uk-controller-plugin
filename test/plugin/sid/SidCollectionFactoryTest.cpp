#include "sid/SidCollectionFactory.h"
#include "sid/SidCollection.h"
#include "sid/StandardInstrumentDeparture.h"

using ::testing::Test;
using UKControllerPlugin::Sid::MakeSidCollection;
using UKControllerPlugin::Sid::SidCollection;
using UKControllerPlugin::Sid::SidDataValid;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;

namespace UKControllerPluginTest::Sid {

    class SidCollectionFactoryTest : public Test
    {
    };

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsTrueAllCorrect)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", 350},
                  {"handoff", 5},
                  {"prenotes", nlohmann::json::array({1, 2})}}),
             nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", nlohmann::json::value_t::null},
                  {"handoff", nlohmann::json::value_t::null},
                  {"prenotes", nlohmann::json::array()}})});

        EXPECT_TRUE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoAirfield)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidAirfield)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", 123},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoIdentifier)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidIdentifier)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", 123},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoInitialAltitude)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidInitialAltitude)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", "6000"},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoInitialHeading)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidInitialHeading)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", "abc"},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInitialHeadingTooSmall)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 0},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInitialHeadingTooBig)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 361},
             {"handoff", 5},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoHandoffId)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 15},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseHandoffIdInvalid)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 15},
             {"handoff", "abc"},
             {"prenotes", nlohmann::json::array({1, 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalsePrenotesMissing)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object({
            {"airfield", "EGGD"},
            {"identifier", "TEST1Y"},
            {"initial_altitude", 6000},
            {"initial_heading", 15},
            {"handoff", 1},
        })});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalsePrenotesNotArray)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 15},
             {"handoff", 1},
             {"prenotes", "abc"}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalsePrenoteNotInteger)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 15},
             {"handoff", 1},
             {"prenotes", nlohmann::json::array({"", 2})}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseDataNotArray)
    {
        EXPECT_FALSE(SidDataValid(nlohmann::json::object()));
    }

    TEST_F(SidCollectionFactoryTest, MakeSidCollectionHandlesBadData)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object(
                 {{"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", 350},
                  {"prenotes", nlohmann::json::array({1, 2})}}),
             nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", nlohmann::json::value_t::null},
                  {"prenotes", nlohmann::json::array({1, 2})}})});

        std::unique_ptr<SidCollection> collection = MakeSidCollection(sidData);
        EXPECT_EQ(0, collection->CountSids());
    }

    TEST_F(SidCollectionFactoryTest, MakeSidCollectionCreatesSids)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", 350},
                  {"handoff", 55},
                  {"prenotes", nlohmann::json::array({1, 2})}}),
             nlohmann::json::object(
                 {{"airfield", "EGLL"},
                  {"identifier", "TEST1Z"},
                  {"initial_altitude", 5000},
                  {"initial_heading", nlohmann::json::value_t::null},
                  {"handoff", nlohmann::json::value_t::null},
                  {"prenotes", nlohmann::json::array()}})});

        std::unique_ptr<SidCollection> collection = MakeSidCollection(sidData);
        EXPECT_EQ(2, collection->CountSids());
        std::shared_ptr<StandardInstrumentDeparture> sid1 = collection->GetByAirfieldAndIdentifier("EGGD", "TEST1Y");
        EXPECT_EQ("EGGD", sid1->Airfield());
        EXPECT_EQ("TEST1Y", sid1->Identifier());
        EXPECT_EQ(6000, sid1->InitialAltitude());
        EXPECT_EQ(350, sid1->InitialHeading());
        EXPECT_EQ(55, sid1->HandoffId());
        EXPECT_EQ(std::set<int>({1, 2}), sid1->Prenotes());

        std::shared_ptr<StandardInstrumentDeparture> sid2 = collection->GetByAirfieldAndIdentifier("EGLL", "TEST1Z");
        EXPECT_EQ("EGLL", sid2->Airfield());
        EXPECT_EQ("TEST1Z", sid2->Identifier());
        EXPECT_EQ(5000, sid2->InitialAltitude());
        EXPECT_EQ(0, sid2->InitialHeading());
        EXPECT_EQ(0, sid2->HandoffId());
        EXPECT_TRUE(sid2->Prenotes().empty());
    }
} // namespace UKControllerPluginTest::Sid
