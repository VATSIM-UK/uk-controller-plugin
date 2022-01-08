#include "metar/MetarComponents.h"
#include "metar/ParsedMetar.h"

using UKControllerPlugin::Metar::MetarComponents;
using UKControllerPlugin::Metar::ParsedMetar;

namespace UKControllerPluginTest::Metar {
    class ParsedMetarTest : public testing::Test
    {
        public:
        ParsedMetarTest()
            : components(std::make_unique<MetarComponents>()), componentsRaw(components.get()),
              metar("EGKK", "foo", std::move(components))
        {
        }

        std::unique_ptr<MetarComponents> components;
        MetarComponents* componentsRaw;
        ParsedMetar metar;
    };

    TEST_F(ParsedMetarTest, ItHasAnAirfield)
    {
        EXPECT_EQ("EGKK", metar.Airfield());
    }

    TEST_F(ParsedMetarTest, ItHasRawForm)
    {
        EXPECT_EQ("foo", metar.Raw());
    }

    TEST_F(ParsedMetarTest, ItHasComponents)
    {
        EXPECT_EQ(componentsRaw, &metar.Components());
    }
} // namespace UKControllerPluginTest::Metar
