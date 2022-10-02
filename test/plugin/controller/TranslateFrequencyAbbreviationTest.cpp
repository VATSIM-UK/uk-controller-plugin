#include "controller/TranslateFrequencyAbbreviation.h"

using UKControllerPlugin::Controller::TranslateFrequencyAbbreviation;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(TranslateFrequencyAbbreviation, ItTranslatesEssexAbbreviation)
        {
            EXPECT_EQ("ESSEX", TranslateFrequencyAbbreviation("ESX"));
        }

        TEST(TranslateFrequencyAbbreviation, ItTranslatesSolentAbbreviation)
        {
            EXPECT_EQ("SOLENT", TranslateFrequencyAbbreviation("SOL"));
        }

        TEST(TranslateFrequencyAbbreviation, ItTranslatesThamesAbbreviation)
        {
            EXPECT_EQ("THAMES", TranslateFrequencyAbbreviation("THA"));
        }

        TEST(TranslateFrequencyAbbreviation, ItTranslatesThamesAbbreviation2)
        {
            EXPECT_EQ("THAMES", TranslateFrequencyAbbreviation("TMS"));
        }

        TEST(TranslateFrequencyAbbreviation, ItReturnsFacilityIfNoTranslation)
        {
            EXPECT_EQ("EGKK", TranslateFrequencyAbbreviation("EGKK"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
