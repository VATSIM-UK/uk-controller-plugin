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
                  {"handoff_id", 5}}),
             nlohmann::json::object(
                 {{"airfield", "EGGD"},
                  {"identifier", "TEST1Y"},
                  {"initial_altitude", 6000},
                  {"initial_heading", nlohmann::json::value_t::null},
                  {"handoff_id", nlohmann::json::value_t::null}})});

        EXPECT_TRUE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoAirfield)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidAirfield)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", 123},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoIdentifier)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidIdentifier)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", 123},
             {"initial_altitude", 6000},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoInitialAltitude)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidInitialAltitude)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", "6000"},
             {"initial_heading", nlohmann::json::value_t::null},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoInitialHeading)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"}, {"identifier", "TEST1Y"}, {"initial_altitude", 6000}, {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInvalidInitialHeading)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", "abc"},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInitialHeadingTooSmall)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 0},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseInitialHeadingTooBig)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 361},
             {"handoff_id", 5}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseNoHandoffId)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object({
            {"airfield", "EGGD"},
            {"identifier", "TEST1Y"},
            {"initial_altitude", 6000},
            {"initial_heading", 15},
        })});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseHandoffIdInvalid)
    {
        nlohmann::json sidData = nlohmann::json::array({nlohmann::json::object(
            {{"airfield", "EGGD"},
             {"identifier", "TEST1Y"},
             {"initial_altitude", 6000},
             {"initial_heading", 15},
             {"handoff_id", "abc"}})});

        EXPECT_FALSE(SidDataValid(sidData));
    }

    TEST_F(SidCollectionFactoryTest, SidDataValidReturnsFalseDataNotArray)
    {
        EXPECT_FALSE(SidDataValid(nlohmann::json::object()));
    }

    TEST_F(SidCollectionFactoryTest, MakeSidCollectionHandlesBadData)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object({
                 {"identifier", "TEST1Y"},
                 {"initial_altitude", 6000},
                 {"initial_heading", 350},
             }),
             nlohmann::json::object({
                 {"airfield", "EGGD"},
                 {"identifier", "TEST1Y"},
                 {"initial_altitude", 6000},
                 {"initial_heading", nlohmann::json::value_t::null},
             })});

        std::unique_ptr<SidCollection> collection = MakeSidCollection(sidData);
        EXPECT_EQ(0, collection->CountSids());
    }

    TEST_F(SidCollectionFactoryTest, MakeSidCollectionCreatesSids)
    {
        nlohmann::json sidData = nlohmann::json::array(
            {nlohmann::json::object({
                 {"airfield", "EGGD"},
                 {"identifier", "TEST1Y"},
                 {"initial_altitude", 6000},
                 {"initial_heading", 350},
                 {"handoff_id", 55},
             }),
             nlohmann::json::object({
                 {"airfield", "EGLL"},
                 {"identifier", "TEST1Z"},
                 {"initial_altitude", 5000},
                 {"initial_heading", nlohmann::json::value_t::null},
                 {"handoff_id", nlohmann::json::value_t::null},
             })});

        std::unique_ptr<SidCollection> collection = MakeSidCollection(sidData);
        EXPECT_EQ(2, collection->CountSids());
        std::shared_ptr<StandardInstrumentDeparture> sid1 = collection->GetByAirfieldAndIdentifier("EGGD", "TEST1Y");
        EXPECT_EQ("EGGD", sid1->Airfield());
        EXPECT_EQ("TEST1Y", sid1->Identifier());
        EXPECT_EQ(6000, sid1->InitialAltitude());
        EXPECT_EQ(350, sid1->InitialHeading());
        EXPECT_EQ(55, sid1->HandoffId());

        std::shared_ptr<StandardInstrumentDeparture> sid2 = collection->GetByAirfieldAndIdentifier("EGLL", "TEST1Z");
        EXPECT_EQ("EGLL", sid2->Airfield());
        EXPECT_EQ("TEST1Z", sid2->Identifier());
        EXPECT_EQ(5000, sid2->InitialAltitude());
        EXPECT_EQ(0, sid2->InitialHeading());
        EXPECT_EQ(0, sid2->HandoffId());
    }
} // namespace UKControllerPluginTest::Sid
