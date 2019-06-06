#include "pch/pch.h"
#include "offblock/EstimatedOffBlockTimeEventHandler.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "flightplan/StoredFlightplan.h"
#include "datablock/DisplayTime.h"

using ::testing::Test;
using ::testing::Return;
using ::testing::NiceMock;
using UKControllerPlugin::Flightplan::StoredFlightplan;
using UKControllerPlugin::Flightplan::StoredFlightplanCollection;
using UKControllerPlugin::Datablock::EstimatedOffBlockTimeEventHandler;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPlugin::Datablock::DisplayTime;

namespace UKControllerPluginTest {
namespace Datablock {

class EstimatedOffBlockTimeEventHandlerTest : public Test
{
    public:
        EstimatedOffBlockTimeEventHandlerTest()
            : handler(this->flightplans, this->timeFormat)
        {

        }

        DisplayTime timeFormat;
        EstimatedOffBlockTimeEventHandler handler;
        StoredFlightplanCollection flightplans;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
};

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItHasATagItemDescription)
{
    EXPECT_TRUE("Estimated Off-block Time" == handler.GetTagItemDescription());
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsEmptyOnNoStoredPlan)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    EXPECT_EQ(
        this->timeFormat.GetUnknownTimeFormat(),
        this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget)
    );
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsDefaultTimeOnNoEobt)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);
    EXPECT_EQ(
        this->timeFormat.GetUnknownTimeFormat(),
        this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget)
    );
}

TEST_F(EstimatedOffBlockTimeEventHandlerTest, TestItReturnsEobt)
{
    ON_CALL(this->mockFlightplan, GetCallsign())
        .WillByDefault(Return("BAW123"));

    ON_CALL(this->mockFlightplan, GetExpectedDepartureTime())
        .WillByDefault(Return("1900"));

    StoredFlightplan storedPlan(this->mockFlightplan);
    this->flightplans.UpdatePlan(storedPlan);
    EXPECT_TRUE("18:45" == this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
}

}  // namespace Datablock
}  // namespace UKControllerPluginTest
