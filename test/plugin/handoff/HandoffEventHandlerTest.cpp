#include "airfield/AirfieldCollection.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/DepartureHandoffResolver.h"
#include "handoff/FlightplanAirfieldHandoffMapper.h"
#include "handoff/FlightplanSidHandoffMapper.h"
#include "handoff/HandoffCache.h"
#include "handoff/HandoffCollection.h"
#include "handoff/HandoffEventHandler.h"
#include "handoff/HandoffFrequencyUpdatedMessage.h"
#include "handoff/HandoffOrder.h"
#include "handoff/ResolvedHandoff.h"
#include "sid/StandardInstrumentDeparture.h"
#include "tag/TagData.h"

using testing::NiceMock;
using testing::Return;
using testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ActiveCallsignCollection;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::DepartureHandoffResolver;
using UKControllerPlugin::Handoff::FlightplanAirfieldHandoffMapper;
using UKControllerPlugin::Handoff::FlightplanSidHandoffMapper;
using UKControllerPlugin::Handoff::HandoffCache;
using UKControllerPlugin::Handoff::HandoffCollection;
using UKControllerPlugin::Handoff::HandoffEventHandler;
using UKControllerPlugin::Handoff::HandoffFrequencyUpdatedMessage;
using UKControllerPlugin::Handoff::HandoffOrder;
using UKControllerPlugin::Handoff::ResolvedHandoff;
using UKControllerPlugin::Sid::StandardInstrumentDeparture;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Integration::MockOutboundIntegrationEventHandler;

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
              handoffMapper(handoffs, sidMapper), airfieldMapper(handoffs, airfields),
              resolver(std::make_shared<DepartureHandoffResolver>(handoffMapper, airfieldMapper, activeCallsigns)),
              cache(std::make_shared<HandoffCache>()), handler(resolver, cache, mockIntegration)
        {
            ON_CALL(this->mockFlightplan, GetCallsign()).WillByDefault(Return("BAW123"));
            ON_CALL(this->mockFlightplan, GetOrigin()).WillByDefault(Return("EGKK"));

            this->hierarchy = std::make_shared<ControllerPositionHierarchy>();
            this->hierarchy->AddPosition(this->position1);
            this->hierarchy->AddPosition(this->position2);

            this->hierarchy2 = std::make_shared<ControllerPositionHierarchy>();
            this->hierarchy2->AddPosition(this->position1);

            ON_CALL(sidMapper, MapFlightplanToSid).WillByDefault(testing::Return(nullptr));
        }

        void AddHandoffOrders()
        {
            ON_CALL(sidMapper, MapFlightplanToSid)
                .WillByDefault(
                    testing::Return(std::make_shared<StandardInstrumentDeparture>(1, 2, "ADMAG2X", 1000, 300, 1)));

            this->handoffs.Add(std::make_shared<HandoffOrder>(1, this->hierarchy));
        }

        double fontSize = 24.1;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo";
        NiceMock<Sid::MockSidMapperInterface> sidMapper;
        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy2;
        NiceMock<MockOutboundIntegrationEventHandler> mockIntegration;
        NiceMock<MockEuroScopeCFlightPlanInterface> mockFlightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> mockRadarTarget;
        AirfieldCollection airfields;
        ActiveCallsignCollection activeCallsigns;
        TagData tagData;
        HandoffCollection handoffs;
        FlightplanSidHandoffMapper handoffMapper;
        FlightplanAirfieldHandoffMapper airfieldMapper;
        std::shared_ptr<DepartureHandoffResolver> resolver;
        std::shared_ptr<HandoffCache> cache;
        HandoffEventHandler handler;
    };

    TEST_F(HandoffEventHandlerTest, TestItReturnsATagItemDescription)
    {
        EXPECT_EQ("Departure Handoff Next Controller", this->handler.GetTagItemDescription(0));
    }

    TEST_F(HandoffEventHandlerTest, TestItReturnsCachedTagItem)
    {
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW123", position2, nullptr, nullptr));
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("132.600", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItReturnsDefaultIfNoHandoffOrder)
    {
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("122.800", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItCachesNoHandoffOrder)
    {
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ(122.800, this->cache->Get("BAW123")->resolvedController->GetFrequency());
    }

    TEST_F(HandoffEventHandlerTest, TestItReturnsUnicomIfNoControllerOnlineInHandoffOrder)
    {
        this->AddHandoffOrders();

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<HandoffFrequencyUpdatedMessage>("BAW123", "122.800");

        EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("122.800", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItCachesNoControllerOnline)
    {
        this->AddHandoffOrders();
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ(122.800, this->cache->Get("BAW123")->resolvedController->GetFrequency());
    }

    TEST_F(HandoffEventHandlerTest, TestItReturnsFrequencyIfControllerFoundInHandoffOrder)
    {
        this->AddHandoffOrders();
        this->activeCallsigns.AddCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", *this->position2, false));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<HandoffFrequencyUpdatedMessage>("BAW123", "132.600");

        EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("132.600", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItCachesFoundController)
    {
        this->AddHandoffOrders();
        this->activeCallsigns.AddCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", *this->position2, false));
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ(132.600, this->cache->Get("BAW123")->resolvedController->GetFrequency());
    }

    TEST_F(HandoffEventHandlerTest, TestItReturnsDefaultIfFoundControllerIsUser)
    {
        this->AddHandoffOrders();
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", *this->position2, true));
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ("122.800", this->tagData.GetItemString());
    }

    TEST_F(HandoffEventHandlerTest, TestItCachesIfFoundControllerIsUser)
    {
        this->AddHandoffOrders();
        this->activeCallsigns.AddUserCallsign(ActiveCallsign("LON_SC_CTR", "Testy McTestFace", *this->position2, true));
        this->handler.SetTagItemData(this->tagData);
        EXPECT_EQ(122.800, this->cache->Get("BAW123")->resolvedController->GetFrequency());
    }

    TEST_F(HandoffEventHandlerTest, TestItClearsCacheOnFlightplanUpdate)
    {
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW123", position2, nullptr, nullptr));
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW456", position2, nullptr, nullptr));
        this->handler.FlightPlanEvent(this->mockFlightplan, this->mockRadarTarget);
        EXPECT_EQ(nullptr, this->cache->Get("BAW123"));
        EXPECT_NE(nullptr, this->cache->Get("BAW456"));
    }

    TEST_F(HandoffEventHandlerTest, TestItClearsCacheOnFlightplanDisconnect)
    {
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW123", position2, nullptr, nullptr));
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW456", position2, nullptr, nullptr));
        this->handler.FlightPlanDisconnectEvent(this->mockFlightplan);
        EXPECT_EQ(nullptr, this->cache->Get("BAW123"));
        EXPECT_NE(nullptr, this->cache->Get("BAW456"));
    }

    TEST_F(HandoffEventHandlerTest, TestItDoesntClearCacheOnFlightplanControllerDataChange)
    {
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW123", position2, nullptr, nullptr));
        this->cache->Add(std::make_shared<ResolvedHandoff>("BAW456", position2, nullptr, nullptr));
        this->handler.ControllerFlightPlanDataEvent(this->mockFlightplan, 1);
        EXPECT_NE(nullptr, this->cache->Get("BAW123"));
        EXPECT_NE(nullptr, this->cache->Get("BAW456"));
    }
} // namespace UKControllerPluginTest::Handoff
