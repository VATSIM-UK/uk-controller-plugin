#include "pch/pch.h"
#include "tag/TagData.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using ::testing::Test;
using ::testing::NiceMock;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest {
    namespace Tag {

        class TagDataTest : public Test
        {
            public:
                TagDataTest()
                    : tagData(
                        mockFlightplan,
                        mockRadarTarget,
                        1,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    )
                {

                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                TagData tagData;
        };

        TEST_F(TagDataTest, ItSetsFlightplanInConstructor)
        {
            EXPECT_EQ(&mockFlightplan, &tagData.flightPlan);
        }

        TEST_F(TagDataTest, ItSetsRadarTargetInConstructor)
        {
            EXPECT_EQ(&mockRadarTarget, &tagData.radarTarget);
        }

        TEST_F(TagDataTest, ItSetsItemCodeInConstructor)
        {
            EXPECT_EQ(1, tagData.itemCode);
        }

        TEST_F(TagDataTest, ItSetsDataAvailableInConstructor)
        {
            EXPECT_EQ(EuroScopePlugIn::TAG_DATA_CORRELATED, tagData.dataAvailable);
        }

        TEST_F(TagDataTest, ItSetsItemStringInConstructor)
        {
            EXPECT_EQ("Foooooo", tagData.GetItemString());
        }

        TEST_F(TagDataTest, ItSetsEuroscopeColourCodeInConstructor)
        {
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_ASSUMED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(TagDataTest, ItSetsTagColourInConstructor)
        {
            EXPECT_EQ(RGB(255, 255, 255), tagData.GetTagColour());
        }

        TEST_F(TagDataTest, ItSetsFontSizeInConstructor)
        {
            EXPECT_EQ(24.1, tagData.GetFontSize());
        }

        TEST_F(TagDataTest, ItSetsEuroscopeColourCode)
        {
            tagData.SetEuroscopeColourCode(EuroScopePlugIn::TAG_COLOR_EMERGENCY);
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_EMERGENCY, tagData.GetEuroscopeColourCode());
        }

        TEST_F(TagDataTest, ItSetsTagColour)
        {
            tagData.SetTagColour(RGB(1, 2, 3));
            EXPECT_EQ(RGB(1, 2, 3), tagData.GetTagColour());
            EXPECT_EQ(EuroScopePlugIn::TAG_COLOR_RGB_DEFINED, tagData.GetEuroscopeColourCode());
        }

        TEST_F(TagDataTest, ItSetsFontSize)
        {
            tagData.SetFontSize(1234.5);
            EXPECT_EQ(1234.5, tagData.GetFontSize());
        }

        TEST_F(TagDataTest, ItSetsItemString)
        {
            tagData.SetItemString("Barrrrrr");
            EXPECT_EQ("Barrrrrr", tagData.GetItemString());
        }

        TEST_F(TagDataTest, ItSetsItemStringToInvalidIfTooLong)
        {
            tagData.SetItemString("thisdataistoolongforthetagitem");
            EXPECT_EQ(tagData.invalidItemText, tagData.GetItemString());
        }
    }  // namespace Tag
}  // namespace UKControllerPluginTest
