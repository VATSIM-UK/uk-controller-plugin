#include "controller/ControllerPosition.h"
#include "handoff/HandoffEventHandler.h"
#include "handoff/ResolvedHandoff.h"
#include "tag/TagData.h"

using testing::NiceMock;
using testing::Return;
using testing::Test;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Handoff::HandoffEventHandler;
using UKControllerPlugin::Handoff::ResolvedHandoff;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;

namespace UKControllerPluginTest::Handoff {

    class HandoffEventHandlerTest : public Test
    {
        public:
        HandoffEventHandlerTest()
            : position1(std::make_shared<ControllerPosition>(
                  1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false)),
              position2(std::make_shared<ControllerPosition>(
                  2, "LON_SC_CTR", 132.6, std::vector<std::string>{}, true, false)),
              tagData(
                  mockFlightplan,
                  mockRadarTarget,
                  1,
                  EuroScopePlugIn::TAG_DATA_CORRELATED,
                  itemString,
                  &euroscopeColourCode,
                  &tagColour,
                  &fontSize),
              mockResolver(std::make_shared<testing::NiceMock<MockDepartureHandoffResolver>>()), handler(mockResolver)
        {
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo";
        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        TagData tagData;
        std::shared_ptr<testing::NiceMock<MockDepartureHandoffResolver>> mockResolver;
        HandoffEventHandler handler;
    };

    TEST_F(HandoffEventHandlerTest, TestItReturnsATagItemDescription)
    {
        EXPECT_EQ("Departure Handoff Next Controller", this->handler.GetTagItemDescription(0));
    }

    TEST_F(HandoffEventHandlerTest, TestItSetsHandoffFrequency)
    {
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("LON_SC_CTR", this->position2, nullptr, nullptr);

        EXPECT_CALL(*this->mockResolver, Resolve(testing::Ref(mockFlightplan))).WillOnce(Return(resolvedHandoff));

        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("132.600", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItHandlesNullptrReturnedByResolver)
    {
        EXPECT_CALL(*this->mockResolver, Resolve(testing::Ref(mockFlightplan))).WillOnce(Return(nullptr));

        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("Foooooo", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, FlightplanEventInvalidatesResolver)
    {
        EXPECT_CALL(*this->mockResolver, Invalidate(testing::Ref(mockFlightplan))).Times(1);

        this->handler.FlightPlanEvent(mockFlightplan, mockRadarTarget);
    }

    TEST_F(HandoffEventHandlerTest, FlightplanDisconnectEventInvalidatesResolver)
    {
        EXPECT_CALL(*this->mockResolver, Invalidate(testing::Ref(mockFlightplan))).Times(1);

        this->handler.FlightPlanDisconnectEvent(mockFlightplan);
    }
} // namespace UKControllerPluginTest::Handoff
