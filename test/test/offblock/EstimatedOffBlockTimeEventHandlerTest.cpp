#include "pch/pch.h"
#include "offblock/EstimatedOffBlockTimeEventHandler.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "datablock/DisplayTime.h"
#include "tag/TagData.h"

using ::testing::Test;
using ::testing::Return;
using ::testing::NiceMock;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Datablock::DisplayTime;
using UKControllerPlugin::Tag::TagData;

namespace UKControllerPluginTest {
namespace Datablock {

class EstimatedOffBlockTimeEventHandlerTest : public Test
{
    public:
        EstimatedOffBlockTimeEventHandlerTest()
            : handler(this->flightplans, this->timeFormat),
            tagData(
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
        DisplayTime timeFormat;
        EstimatedOffBlockTimeEventHandler handler;
        StoredFlightplanCollection flightplans;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        TagData tagData;
};

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItHasATagItemDescription)
{
    EXPECT_TRUE("Estimated Off-block Time" == handler.GetTagItemDescription());
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsEmptyOnNoStoredPlan)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    handler.SetTagItemData(this->tagData);
    EXPECT_EQ(this->timeFormat.GetUnknownTimeFormat(), this->tagData.GetItemString());
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsDefaultTimeOnNoEobt)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);
    handler.SetTagItemData(this->tagData);
    EXPECT_EQ(this->timeFormat.GetUnknownTimeFormat(), this->tagData.GetItemString());
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsEobt)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    ON_CALL(this->mockFlightplan, GetExpectedDepartureTime())
        .WillByDefault(Return("1900"));

    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);
    handler.SetTagItemData(this->tagData);
    EXPECT_EQ("18:45", this->tagData.GetItemString());
}

}  // namespace Datablock
}  // namespace UKControllerPluginTest
