#include "selcal/ParsedSelcal.h"

using UKControllerPlugin::Selcal::ParsedSelcal;

namespace UKControllerPluginTest::Selcal {
    class ParsedSelcalTest : public testing::Test
    {
        public:
        ParsedSelcalTest() : selcal("ABCD")
        {
        }
        ParsedSelcal selcal;
    };

    TEST_F(ParsedSelcalTest, ItReturnsRawSelcal)
    {
        EXPECT_EQ("ABCD", selcal.GetRaw());
    }

    TEST_F(ParsedSelcalTest, ItReturnsSelcalWithSeparator)
    {
        EXPECT_EQ("AB-CD", selcal.GetWithSeparator());
    }
} // namespace UKControllerPluginTest::Selcal
