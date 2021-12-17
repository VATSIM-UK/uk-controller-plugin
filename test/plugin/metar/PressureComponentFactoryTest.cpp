#include "metar/MetarComponents.h"
#include "metar/PressureComponent.h"
#include "metar/PressureComponentFactory.h"

using UKControllerPlugin::Metar::MetarComponents;
using UKControllerPlugin::Metar::PressureComponent;
using UKControllerPlugin::Metar::PressureComponentFactory;
using UKControllerPlugin::Metar::PressureUnit;

namespace UKControllerPluginTest::Metar {
    class PressureComponentFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetJson(const nlohmann::json& overridingData = nlohmann::json::object(), const std::string& keyToRemove = "")
            -> nlohmann::json
        {
            nlohmann::json json{
                {"qnh", 1013}, {"qfe", 1007}, {"qnh_inhg", 29.92}, {"qfe_inhg", 28.21}, {"pressure_format", "hpa"}};
            if (overridingData.is_object()) {
                json.update(overridingData);
            } else {
                json = overridingData;
            }

            if (!keyToRemove.empty()) {
                json.erase(json.find(keyToRemove));
            }

            return json;
        }

        MetarComponents components;
        PressureComponentFactory factory;
    };

    TEST_F(PressureComponentFactoryTest, ItCreatesAHectopascalComponent)
    {
        factory.FromJson(GetJson(), components);
        EXPECT_NE(nullptr, components.pressure);
        EXPECT_EQ(1013, components.pressure->QnhHectopascals());
        EXPECT_EQ(1007, components.pressure->QfeHectopascals());
        EXPECT_FLOAT_EQ(29.92, components.pressure->QnhInHg());
        EXPECT_FLOAT_EQ(28.21, components.pressure->QfeInHg());
        EXPECT_TRUE(components.pressure->ReportedAsHectopascals());
    }

    TEST_F(PressureComponentFactoryTest, ItCreatesAnInhgComponent)
    {
        factory.FromJson(GetJson(nlohmann::json{{"pressure_format", "inhg"}}), components);
        EXPECT_NE(nullptr, components.pressure);
        EXPECT_EQ(1013, components.pressure->QnhHectopascals());
        EXPECT_EQ(1007, components.pressure->QfeHectopascals());
        EXPECT_FLOAT_EQ(29.92, components.pressure->QnhInHg());
        EXPECT_FLOAT_EQ(28.21, components.pressure->QfeInHg());
        EXPECT_TRUE(components.pressure->ReportedAsInHg());
    }

    TEST_F(PressureComponentFactoryTest, ItCreatesAnInhgComponentWithNonFloatValues)
    {
        factory.FromJson(
            GetJson(nlohmann::json{{"pressure_format", "inhg"}, {"qnh_inhg", 29}, {"qfe_inhg", 28}}), components);
        EXPECT_NE(nullptr, components.pressure);
        EXPECT_EQ(1013, components.pressure->QnhHectopascals());
        EXPECT_EQ(1007, components.pressure->QfeHectopascals());
        EXPECT_FLOAT_EQ(29, components.pressure->QnhInHg());
        EXPECT_FLOAT_EQ(28, components.pressure->QfeInHg());
        EXPECT_TRUE(components.pressure->ReportedAsInHg());
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrInvalidPressureFormat)
    {
        factory.FromJson(GetJson(nlohmann::json{{"pressure_format", "bar"}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrPressureFormatNotString)
    {
        factory.FromJson(GetJson(nlohmann::json{{"pressure_format", 123}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrPressureFormatMissing)
    {
        factory.FromJson(GetJson(nlohmann::json::object(), "pressure_format"), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQnhHectopascalsNotInteger)
    {
        factory.FromJson(GetJson(nlohmann::json{{"qnh", 12.23}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQnhHectopascalsMissing)
    {
        factory.FromJson(GetJson(nlohmann::json::object(), "qnh"), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQfeHectopascalsNotInteger)
    {
        factory.FromJson(GetJson(nlohmann::json{{"qfe", 12.23}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQfeHectopascalsMissing)
    {
        factory.FromJson(GetJson(nlohmann::json::object(), "qfe"), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQnhInhgNotNumber)
    {
        factory.FromJson(GetJson(nlohmann::json{{"qnh_inhg", "abc"}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQnhInHgMissing)
    {
        factory.FromJson(GetJson(nlohmann::json::object(), "qnh_inhg"), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQfeInhgNotNumber)
    {
        factory.FromJson(GetJson(nlohmann::json{{"qfe_inhg", "abc"}}), components);
        EXPECT_EQ(nullptr, components.pressure);
    }

    TEST_F(PressureComponentFactoryTest, ItReturnsNullptrQfeInHgMissing)
    {
        factory.FromJson(GetJson(nlohmann::json::object(), "qfe_inhg"), components);
        EXPECT_EQ(nullptr, components.pressure);
    }
} // namespace UKControllerPluginTest::Metar
