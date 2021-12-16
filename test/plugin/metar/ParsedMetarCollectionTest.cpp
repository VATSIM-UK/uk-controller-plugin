#include "metar/MetarComponents.h"
#include "metar/ParsedMetar.h"
#include "metar/ParsedMetarCollection.h"

using UKControllerPlugin::Metar::ParsedMetar;
using UKControllerPlugin::Metar::ParsedMetarCollection;

namespace UKControllerPluginTest::Metar {
    class ParsedMetarCollectionTest : public testing::Test
    {
        public:
        ParsedMetarCollectionTest()
            : metar1(std::make_shared<ParsedMetar>("EGKK", "foo", nullptr)),
              metar2(std::make_shared<ParsedMetar>("EGLL", "bar", nullptr)),
              metar1a(std::make_shared<ParsedMetar>("EGKK", "baz", nullptr))
        {
        }

        std::shared_ptr<ParsedMetar> metar1;
        std::shared_ptr<ParsedMetar> metar2;
        std::shared_ptr<ParsedMetar> metar1a;
        ParsedMetarCollection metars;
    };

    TEST_F(ParsedMetarCollectionTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, metars.Count());
    }

    TEST_F(ParsedMetarCollectionTest, ItAddsMetars)
    {
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar2);

        EXPECT_EQ(2, metars.Count());
        EXPECT_EQ(metar1, metars.GetForAirfield("EGKK"));
        EXPECT_EQ(metar2, metars.GetForAirfield("EGLL"));
    }

    TEST_F(ParsedMetarCollectionTest, ItDoesntDuplicateMetars)
    {
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar2);
        metars.UpdateMetar(metar2);
        metars.UpdateMetar(metar2);
    }

    TEST_F(ParsedMetarCollectionTest, ItUpdatesMetars)
    {
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar2);
        metars.UpdateMetar(metar1a);

        EXPECT_EQ(2, metars.Count());
        EXPECT_EQ(metar1a, metars.GetForAirfield("EGKK"));
        EXPECT_EQ(metar2, metars.GetForAirfield("EGLL"));
    }

    TEST_F(ParsedMetarCollectionTest, ItReturnsNullptrIfMetarNotFound)
    {
        metars.UpdateMetar(metar1);
        metars.UpdateMetar(metar2);

        EXPECT_EQ(nullptr, metars.GetForAirfield("EGBB"));
    }
} // namespace UKControllerPluginTest::Metar
