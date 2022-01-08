#include "metar/MetarComponents.h"
#include "metar/MetarComponentsFactory.h"
#include "metar/MetarComponentsFactoryFactory.h"
#include "metar/PressureComponent.h"

using UKControllerPlugin::Metar::BuildComponentsFactory;
using UKControllerPlugin::Metar::MetarComponents;
using UKControllerPlugin::Metar::MetarComponentsFactory;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {
    class MetarComponentsFactoryTest : public testing::Test
    {
        public:
        MetarComponentsFactoryTest() : factory(BuildComponentsFactory())
        {
        }

        MetarComponents components;
        std::shared_ptr<MetarComponentsFactory> factory;
    };

    TEST_F(MetarComponentsFactoryTest, ItReturnsComponents)
    {
        nlohmann::json json{
            {"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}};
        auto components = factory->FromJson(json);

        EXPECT_NE(nullptr, components);
        EXPECT_EQ(1013, components->pressure->QnhHectopascals());
    }

    TEST_F(MetarComponentsFactoryTest, ItReturnsComponentsEmptyData)
    {
        auto components = factory->FromJson(nlohmann::json::object());
        EXPECT_NE(nullptr, components);
        EXPECT_EQ(nullptr, components->pressure);
    }

    TEST_F(MetarComponentsFactoryTest, ItReturnsNullptrIfJsonNotArray)
    {
        nlohmann::json json{
            {"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}};

        EXPECT_NE(nullptr, factory->FromJson(nlohmann::json::array()));
    }
} // namespace UKControllerPluginTest::Metar
