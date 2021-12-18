#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldModel.h"
#include "controller/ControllerPositionHierarchy.h"
#include "metar/MetarsUpdatedPushEventProcessor.h"
#include "metar/MetarComponents.h"
#include "metar/MetarComponentsFactory.h"
#include "metar/MetarComponentsFactoryFactory.h"
#include "metar/ParsedMetar.h"
#include "metar/ParsedMetarCollection.h"
#include "metar/ParsedMetarFactory.h"
#include "metar/PressureComponent.h"
#include "push/PushEvent.h"
#include "push/PushEventSubscription.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPlugin::Metar::BuildComponentsFactory;
using UKControllerPlugin::Metar::MetarComponentsFactory;
using UKControllerPlugin::Metar::MetarsUpdatedPushEventProcessor;
using UKControllerPlugin::Metar::ParsedMetarCollection;
using UKControllerPlugin::Metar::ParsedMetarFactory;
using UKControllerPlugin::Push::PushEvent;
using UKControllerPlugin::Push::PushEventSubscription;

namespace UKControllerPluginTest::Metar {
    class MetarsUpdatedPushEventProcessorTest : public testing::Test
    {
        public:
        MetarsUpdatedPushEventProcessorTest()
            : componentsFactory(BuildComponentsFactory()), factory(*componentsFactory, airfields),
              processor(collection, factory)
        {
            this->airfields.AddAirfield(std::make_shared<AirfieldModel>(1, "EGKK", nullptr));
            this->airfields.AddAirfield(std::make_shared<AirfieldModel>(2, "EGLL", nullptr));
        }

        /*
         * Make an event based on the merge of some base data and overriding data so we dont
         * have to repeat ourselves
         */
        static PushEvent MakePushEvent(const nlohmann::json& data)
        {
            return {"metars,updated.created", "test", data, data.dump()};
        };

        std::shared_ptr<MetarComponentsFactory> componentsFactory;
        AirfieldCollection airfields;
        ParsedMetarFactory factory;
        ParsedMetarCollection collection;
        MetarsUpdatedPushEventProcessor processor;
    };

    TEST_F(MetarsUpdatedPushEventProcessorTest, ItHasPushEventSubscriptions)
    {
        std::set<PushEventSubscription> expected{{PushEventSubscription::SUB_TYPE_EVENT, "metars.updated"}};
        EXPECT_EQ(expected, processor.GetPushEventSubscriptions());
    }

    TEST_F(MetarsUpdatedPushEventProcessorTest, ItHandlesNonArrayData)
    {
        nlohmann::json data = {
            {"airfield_id", 1},
            {"raw", "foo"},
            {"parsed",
             {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}};
        processor.ProcessPushEvent(MakePushEvent(data));

        EXPECT_EQ(0, collection.Count());
    }

    TEST_F(MetarsUpdatedPushEventProcessorTest, ItCreatesParsedMetars)
    {
        nlohmann::json data = nlohmann::json::array();
        data.push_back(
            {{"airfield_id", 1},
             {"raw", "foo"},
             {"parsed",
              {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}});
        data.push_back(
            {{"airfield_id", 2},
             {"raw", "bar"},
             {"parsed",
              {{"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}}}});

        processor.ProcessPushEvent(MakePushEvent(data));

        EXPECT_EQ(2, collection.Count());
        EXPECT_NE(nullptr, collection.GetForAirfield("EGKK"));
        EXPECT_EQ("foo", collection.GetForAirfield("EGKK")->Raw());
        EXPECT_NE(nullptr, collection.GetForAirfield("EGLL"));
        EXPECT_EQ("bar", collection.GetForAirfield("EGLL")->Raw());
    }
} // namespace UKControllerPluginTest::Metar
