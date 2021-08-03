#include "pch/pch.h"
#include "offblock/EstimatedDepartureTimeEventHandler.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "helper/HelperFunctions.h"
#include "datablock/DisplayTime.h"
#include "tag/TagData.h"

using ::testing::Test;
using ::testing::Return;
using ::testing::NiceMock;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Datablock::EstimatedDepartureTimeEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::HelperFunctions;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPluginTest {
namespace Datablock {

class EstimatedDepartureTimeEventHandlerTest : public Test
{
    public:
        EstimatedDepartureTimeEventHandlerTest()
            : handler(this->flightplans, timeFormat),
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
            ON_CALL(this->mockFlightplan, GetCallsign())
                .WillByDefault(Return("BAW123"));

            ON_CALL(this->mockFlightplan, GetExpectedDepartureTime())
                .WillByDefault(Return("1900"));
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo";
        DisplayTime timeFormat;
        EstimatedDepartureTimeEventHandler handler;
        StoredFlightplanCollection flightplans;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        TagData tagData;
};

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItHasATagItemDescription)
{
    EXPECT_TRUE("Estimated Departure Time" == handler.GetTagItemDescription(0));
}

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItDoesntUpdateOnNoStoredPlan)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    EXPECT_NO_THROW(this->handler.FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget));
}

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItUpdatesEdtFromFlightplan)
{
    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);

    this->handler.FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget);
    EXPECT_NE(
        (std::chrono::system_clock::time_point::max)(),
        this->flightplans.GetFlightplanForCallsign("BAW123").GetEstimatedDepartureTime()
    );

    handler.SetTagItemData(this->tagData);
    EXPECT_EQ("19:00", this->tagData.GetItemString());
}

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItReturnsEmptyOnNoStoredPlan)
{
    handler.SetTagItemData(this->tagData);
    EXPECT_EQ(this->timeFormat.GetUnknownTimeFormat(), this->tagData.GetItemString());
}

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItReturnsEmptyOnNoEdt)
{
    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);
    handler.SetTagItemData(this->tagData);
    EXPECT_EQ(this->timeFormat.GetUnknownTimeFormat(), this->tagData.GetItemString());
}

TEST_F(EstimatedDepartureTimeEventHandlerTest, TestItReturnsEdt)
{
    StoredFlightplan storedPlan(this->mockFlightplan);
    storedPlan.SetEstimatedDepartureTime(HelperFunctions::GetTimeFromNumberString("1900"));
    this->flightplans.UpdatePlan(storedPlan);
    handler.SetTagItemData(this->tagData);
    EXPECT_EQ("19:00", this->tagData.GetItemString());
}

}  // namespace Datablock
}  // namespace UKControllerPluginTest
