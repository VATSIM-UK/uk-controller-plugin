#include "sid/NormaliseSid.h"

using UKControllerPlugin::Sid::NormaliseSid;

namespace UKControllerPluginTest::Sid {

    class NormaliseSidTest : public testing::Test
    {
        public:
        NormaliseSid normalise;
    };

    TEST_F(NormaliseSidTest, StripSidDeprecationHandlesEmptyString)
    {
        std::string expected = "";
        ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation("")));
    }

    TEST_F(NormaliseSidTest, StripSidDeprecationRemovesDeprecationMarkerAtStart)
    {
        std::string expected = "ADMAG1X";
        ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation(normalise.sidDeprecationPrefix + expected)));
    }

    TEST_F(NormaliseSidTest, StripSidDeprecationRemovesDeprecationMarkerAtEnd)
    {
        std::string expected = "ADMAG1X";
        ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation(expected + normalise.sidDeprecationPrefix)));
    }

    TEST_F(NormaliseSidTest, StripSidDeprecationRemovesDeprecationMarkerInMiddle)
    {
        std::string expected = "TEST1X";
        ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation("TEST#1X")));
    }

    TEST_F(NormaliseSidTest, StripSidDeprecationDoesNotAlterNonDeprecated)
    {
        std::string expected = "TEST1X";
        ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation("TEST1X")));
    }
} // namespace UKControllerPluginTest::Sid
