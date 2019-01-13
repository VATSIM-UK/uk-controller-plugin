#include "pch/pch.h"
#include "airfield/NormaliseSid.h"

using UKControllerPlugin::Airfield::NormaliseSid;

namespace UKControllerPluginTest {
    namespace Airfield {

        TEST(SidParser, StripSidDeprecationRemovesDeprecationMarkerAtStart)
        {
            NormaliseSid normalise;
            std::string expected = "ADMAG1X";
            ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation(normalise.sidDeprecationPrefix + expected)));
        }

        TEST(SidParser, StripSidDeprecationRemovesDeprecationMarkerAtEnd)
        {
            NormaliseSid normalise;
            std::string expected = "ADMAG1X";
            ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation(expected + normalise.sidDeprecationPrefix)));
        }

        TEST(SidParser, StripSidDeprecationRemovesDeprecationMarkerInMiddle)
        {
            NormaliseSid normalise;
            std::string expected = "TEST1X";
            ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation("TEST#1X")));
        }

        TEST(SidParser, StripSidDeprecationDoesNotAlterNonDeprecated)
        {
            NormaliseSid normalise;
            std::string expected = "TEST1X";
            ASSERT_EQ(0, expected.compare(normalise.StripSidDeprecation("TEST1X")));
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest
