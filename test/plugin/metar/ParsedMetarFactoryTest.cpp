#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "metar/MetarComponents.h"
#include "metar/MetarComponentsFactory.h"
#include "metar/MetarComponentsFactoryFactory.h"
#include "metar/ParsedMetar.h"
#include "metar/ParsedMetarFactory.h"
#include "metar/PressureComponent.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Metar::BuildComponentsFactory;
using UKControllerPlugin::Metar::MetarComponentsFactory;
using UKControllerPlugin::Metar::ParsedMetarFactory;

namespace UKControllerPluginTest::Metar {
    class ParsedMetarFactoryTest : public testing::Test
    {
        public:
        ParsedMetarFactoryTest() : componentsFactory(BuildComponentsFactory()), factory(*componentsFactory, airfields)
        {
            this->airfields.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", nullptr));
        }

        std::shared_ptr<MetarComponentsFactory> componentsFactory;
        AirfieldCollection airfields;
        ParsedMetarFactory factory;
    };

    TEST_F(ParsedMetarFactoryTest, ItReturnsParsedMetar)
    {
        nlohmann::json data = {
            {"airfield_id", 1},
            {"raw", "foo"},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        auto metar = factory.FromJson(data);
        EXPECT_EQ("EGKK", metar->Airfield());
        EXPECT_EQ("foo", metar->Raw());
        EXPECT_EQ(1013, metar->Components().pressure->QnhHectopascals());
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrAirfieldMissing)
    {
        nlohmann::json data = {
            {"raw", "foo"},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrAirfieldNotInteger)
    {
        nlohmann::json data = {
            {"airfield_id", "abc"},
            {"raw", "foo"},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrAirfieldNotValid)
    {
        nlohmann::json data = {
            {"airfield_id", 2},
            {"raw", "foo"},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrRawMissing)
    {
        nlohmann::json data = {
            {"airfield_id", 1},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrRawNotString)
    {
        nlohmann::json data = {
            {"airfield_id", 1},
            {"raw", 123},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrParsedMissing)
    {
        nlohmann::json data = {
            {"airfield_id", 1},
            {"raw", "foo"},
        };

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrParsedNotObject)
    {
        nlohmann::json data = {{"airfield_id", 1}, {"raw", "foo"}, {"parsed", nlohmann::json::array()}};

        EXPECT_EQ(nullptr, factory.FromJson(data));
    }

    TEST_F(ParsedMetarFactoryTest, ItReturnsNullPtrMessageNotObject)
    {
        EXPECT_EQ(nullptr, factory.FromJson(nlohmann::json::array()));
    }
} // namespace UKControllerPluginTest::Metar
