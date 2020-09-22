#include "pch/pch.h"
#include "stands/StandEventHandler.h"
#include "stands/CompareStands.h"
#include "stands/Stand.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"

using ::testing::Test;
using UKControllerPlugin::Stands::StandEventHandler;
using UKControllerPlugin::Stands::CompareStands;
using UKControllerPlugin::Stands::Stand;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Stands {

        class StandEventHandlerTest : public Test
        {
            public:
                StandEventHandlerTest()
                    : handler(this->GetStands()),
                    tagData(flightplan, radarTarget, 110, 1, itemString, &euroscopeColourCode, &tagColour, &fontSize)
                {
                    ON_CALL(this->flightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));
                }

                std::set<Stand, CompareStands> GetStands(void) const
                {
                    std::set<Stand, CompareStands> stands;
                    stands.insert({ 1, "EGKK", "1L" });
                    stands.insert({ 2, "EGKK", "55" });
                    stands.insert({ 3, "EGLL", "317" });
                    return stands;
                }

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
                TagData tagData;
                StandEventHandler handler;
        };

        TEST_F(StandEventHandlerTest, ItStartsWithNoAssignments)
        {
            EXPECT_EQ(0, this->handler.CountStandAssignments());
        }

        TEST_F(StandEventHandlerTest, ItReturnsATagItemDescription)
        {
            EXPECT_EQ("Assigned Stand", this->handler.GetTagItemDescription(110));
        }

        TEST_F(StandEventHandlerTest, ItReturnsAssignedStand)
        {
            this->handler.SetAssignedStand("BAW123", 3);
            this->handler.SetTagItemData(this->tagData);
            EXPECT_EQ("317", this->tagData.GetItemString());
        }

        TEST_F(StandEventHandlerTest, ItReturnsNothingIfStandNotAssigned)
        {
            this->handler.SetTagItemData(this->tagData);
            EXPECT_EQ("Foooooo", this->tagData.GetItemString());
        }
    }  // namespace Stands
}  // namespace UKControllerPluginTest
