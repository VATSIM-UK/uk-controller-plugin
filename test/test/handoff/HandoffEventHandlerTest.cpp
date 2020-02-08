#include "pch/pch.h"
#include "handoff/HandoffEventHandler.h"
#include "handoff/HandoffCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"

using UKControllerPlugin::Handoff::HandoffEventHandler;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using testing::Test;
using testing::NiceMock;
using testing::Return;

namespace UKControllerPluginTest {
    namespace Handoff {

        class HandoffEventHandlerTest : public Test
        {
            public:
                HandoffEventHandlerTest()
                    : handler(handoffs, activeCallsigns), position1("LON_S_CTR", 129.420, "CTR", {}),
                    position2("LON_SC_CTR", 132.6, "CTR", {})
                {
                    ON_CALL(this->mockFlightplan, GetCallsign())
                        .WillByDefault(Return("BAW123"));

                    ON_CALL(this->mockFlightplan, GetOrigin())
                        .WillByDefault(Return("EGKK"));

                    ON_CALL(this->mockFlightplan, GetSidName())
                        .WillByDefault(Return("ADMAG2X"));

                    this->hierarchy.reset(new ControllerPositionHierarchy);
                    this->hierarchy->AddPosition(this->position1);
                    this->hierarchy->AddPosition(this->position2);
                }

                ControllerPosition position1;
                ControllerPosition position2;
                std::shared_ptr<ControllerPositionHierarchy> hierarchy;
                NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
                NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
                HandoffCollection handoffs;
                ActiveCallsignCollection activeCallsigns;
                HandoffEventHandler handler;
        };

        TEST_F(HandoffEventHandlerTest, TestItReturnsATagItemDescription)
        {
            EXPECT_EQ("Departure Handoff Next Controller", this->handler.GetTagItemDescription());
        }

        TEST_F(HandoffEventHandlerTest, TestItReturnsCachedTagItem)
        {
            this->handler.AddCachedItem("BAW123", "123.456");
            EXPECT_EQ("123.456", this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
        }

        TEST_F(HandoffEventHandlerTest, TestItReturnsDefaultIfNoHandoffOrder)
        {
            EXPECT_EQ(handler.DEFAULT_TAG_VALUE, this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
        }

        TEST_F(HandoffEventHandlerTest, TestItCachesNoHandoffOrder)
        {
            this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget);
            EXPECT_EQ(handler.DEFAULT_TAG_VALUE, this->handler.GetCachedItem("BAW123"));
        }

        TEST_F(HandoffEventHandlerTest, TestItReturnsDefaultIfNoControllerOnlineInHandoffOrder)
        {
            this->handoffs.AddHandoffOrder("EGKK_ADMAG2X", this->hierarchy);
            this->handoffs.AddSidMapping("EGKK", "ADMAG2K", "EGKK_ADMAG2X");
            EXPECT_EQ(handler.DEFAULT_TAG_VALUE, this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
        }

        TEST_F(HandoffEventHandlerTest, TestItCachesNoControllerOnline)
        {
            this->handoffs.AddHandoffOrder("EGKK_ADMAG2X", this->hierarchy);
            this->handoffs.AddSidMapping("EGKK", "ADMAG2K", "EGKK_ADMAG2X");
            this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget);
            EXPECT_EQ(handler.DEFAULT_TAG_VALUE, this->handler.GetCachedItem("BAW123"));
        }

        TEST_F(HandoffEventHandlerTest, TestItReturnsFrequencyIfControllerFoundInHandoffOrder)
        {
            this->handoffs.AddHandoffOrder("EGKK_ADMAG2X", this->hierarchy);
            this->handoffs.AddSidMapping("EGKK", "ADMAG2X", "EGKK_ADMAG2X");
            this->activeCallsigns.AddCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", this->position2));
            EXPECT_EQ("132.600", this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget));
        }

        TEST_F(HandoffEventHandlerTest, TestItCachesFoundController)
        {
            this->handoffs.AddHandoffOrder("EGKK_ADMAG2X", this->hierarchy);
            this->handoffs.AddSidMapping("EGKK", "ADMAG2X", "EGKK_ADMAG2X");
            this->activeCallsigns.AddCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", this->position2));
            this->handler.GetTagItemData(this->mockFlightplan, this->mockRadarTarget);
            EXPECT_EQ("132.600", this->handler.GetCachedItem("BAW123"));
        }

    }  // namespace Handoff
}  // namespace UKControllerPluginTest
