#include "pch/pch.h"
#include "flightinformationservice/FlightInformationServiceTagItem.h"
#include "tag/TagData.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using ::testing::Test;
using UKControllerPlugin::FlightInformationService::FlightInformationServiceTagItem;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest {
    namespace FlightInformationService {

        class FlightInformationServiceTagItemTest : public Test
        {
            public:

                FlightInformationServiceTagItemTest()
                {
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(testing::Return("BAW123"));
                }

                TagData GetTagData(int tagItemId)
                {
                    return TagData(
                        flightplan,
                        radarTarget,
                        tagItemId,
                        EuroScopePlugIn::TAG_DATA_CORRELATED,
                        itemString,
                        &euroscopeColourCode,
                        &tagColour,
                        &fontSize
                    );
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                testing::NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                testing::NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
                FlightInformationServiceTagItem tagItem;
        };

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBaseTagItem)
        {
            EXPECT_EQ("Flight Information Service", tagItem.GetTagItemDescription(116));
        }

        TEST_F(FlightInformationServiceTagItemTest, ItDescribesTheBlankOptionTagItem)
        {
            EXPECT_EQ("Flight Information Service or Blank", tagItem.GetTagItemDescription(117));
        }

        TEST_F(FlightInformationServiceTagItemTest, ItSetsTagItemDataForBaseTagItem)
        {
            auto tagData = this->GetTagData(116);
            tagItem.SetServiceForAircraft("BAW123", "BASIC");
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("BASIC", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItReturnsNoDataForBaseTagItem)
        {
            auto tagData = this->GetTagData(116);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("UKFIS", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItSetsTagItemDataForOrBlankTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetServiceForAircraft("BAW123", "TFC");
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("TFC", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, ItReturnsNoDataForOrBlankTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
        }

        TEST_F(FlightInformationServiceTagItemTest, FlightplanDisconnectingClearsTagItem)
        {
            auto tagData = this->GetTagData(117);
            tagItem.SetServiceForAircraft("BAW123", "TFC");

            tagItem.FlightPlanDisconnectEvent(this->flightplan);
            tagItem.SetTagItemData(tagData);

            EXPECT_EQ("", tagData.GetItemString());
        }
    }  // namespace FlightInformationService
}  // namespace UKControllerPluginTest
