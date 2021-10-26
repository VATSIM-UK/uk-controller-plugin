#include "selcal/ParsedSelcal.h"
#include "selcal/SelcalParser.h"

using UKControllerPlugin::Selcal::ParsedSelcal;
using UKControllerPlugin::Selcal::SelcalParser;

namespace UKControllerPluginTest::Selcal {
    class SelcalParserTest : public testing::Test
    {
        public:
        SelcalParser parser;
    };

    TEST_F(SelcalParserTest, ItParsesCharactersAThroughD)
    {
        EXPECT_EQ("ABCD", parser.ParseFromString("SEL/ABCD")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesCharactersEThroughH)
    {
        EXPECT_EQ("EFGH", parser.ParseFromString("SEL/EFGH")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesCharactersJThroughM)
    {
        EXPECT_EQ("JKLM", parser.ParseFromString("SEL/JKLM")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesCharactersPThroughS)
    {
        EXPECT_EQ("PQRS", parser.ParseFromString("SEL/PQRS")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesWithDataBefore)
    {
        EXPECT_EQ("ABCD", parser.ParseFromString("RMK/HI SEL/ABCD")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesCharactersWithSecondPairLowerThanFirst)
    {
        EXPECT_EQ("CDAB", parser.ParseFromString("SEL/CDAB")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesWithDataAfter)
    {
        EXPECT_EQ("ABCD", parser.ParseFromString("SEL/ABCD RMK/HI")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItParsesWithDataBeforeAndAfter)
    {
        EXPECT_EQ("ABCD", parser.ParseFromString("RMK/HI SEL/ABCD RMK/HI")->GetRaw());
    }

    TEST_F(SelcalParserTest, ItDoesntParseInvalidCharacter)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SEL/PQRZ"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseIfItCantFindKey)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("PQRS"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseInvalidKey)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SELL/PQRS"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseExtraCharacterAfter)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SEL/PQRST"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseExtraCharacterBefore)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("ASEL/PQRST"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseDuplicateCharacter)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SEL/AABC"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseIfFirstPairWrongOrder)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SEL/BACD"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseIfSecondPairWrongOrder)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("SEL/ABDC"));
    }

    TEST_F(SelcalParserTest, ItDoesntParseIfRubbish)
    {
        EXPECT_EQ(nullptr, parser.ParseFromString("ABSADSAJD"));
    }
} // namespace UKControllerPluginTest::Selcal
