#include "bootstrap/PersistenceContainer.h"
#include "intention/IntentionCodeCache.h"
#include "intention/IntentionCodeEventHandler.h"
#include "intention/IntentionCodeFactory.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/IntentionCodeUpdatedMessage.h"
#include "intention/SectorExitRepositoryFactory.h"
#include "intention/SectorExitRepository.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "mock/MockOutboundIntegrationEventHandler.h"
#include "tag/TagData.h"

using testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandler;
using UKControllerPlugin::IntentionCode::IntentionCodeFactory;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeUpdatedMessage;
using UKControllerPlugin::IntentionCode::SectorExitRepository;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;
using UKControllerPlugin::Tag::TagData;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using UKControllerPluginTest::Integration::MockOutboundIntegrationEventHandler;

namespace UKControllerPluginTest::IntentionCode {

    class IntentionCodeEventHandlerTest : public Test
    {
        public:
        void SetUp() override
        {
            auto cache = std::make_unique<IntentionCodeCache>();
            cache->RegisterAircraft("BAW456", {"KK", false, -1, ""});
            PersistenceContainer container;
            this->repository = SectorExitRepositoryFactory::Create();
            this->handler = std::make_unique<IntentionCodeEventHandler>(
                IntentionCodeFactory::Create(*this->repository), std::move(cache), mockIntegration);
        };

        const double FONT_SIZE = 24.1;
        double fontSize = FONT_SIZE;
        COLORREF tagColour = RGB(255, 255, 255);
        int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
        char itemString[16] = "Foooooo"; // NOLINT
        NiceMock<MockOutboundIntegrationEventHandler> mockIntegration;
        std::unique_ptr<SectorExitRepository> repository;
        std::unique_ptr<IntentionCodeEventHandler> handler;
    };

    TEST_F(IntentionCodeEventHandlerTest, GetTagItemDescriptionReturnsCorrectDescription)
    {
        EXPECT_TRUE("UKCP Intention Code" == this->handler->GetTagItemDescription(0));
    }

    TEST_F(IntentionCodeEventHandlerTest, GetTagItemDataGeneratesIntentionCodeIfNonePresent)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> route;
        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        TagData tagData(
            flightplan,
            radarTarget,
            1,
            EuroScopePlugIn::TAG_DATA_CORRELATED,
            itemString,
            &euroscopeColourCode,
            &tagColour,
            &fontSize);

        EXPECT_CALL(flightplan, GetExtractedRoute()).Times(1).WillOnce(Return(ByRef(route)));

        ON_CALL(flightplan, GetCallsign).WillByDefault(Return("BAW123"));

        EXPECT_CALL(flightplan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

        EXPECT_CALL(flightplan, GetDestination()).Times(1).WillOnce(Return("EGLL"));

        EXPECT_CALL(flightplan, GetCruiseLevel()).Times(1).WillOnce(Return(8000));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "LL");

        EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        handler->SetTagItemData(tagData);
        EXPECT_EQ("LL", tagData.GetItemString());
    }

    TEST_F(IntentionCodeEventHandlerTest, GetTagItemDataCachesCodeAndReusesIt)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> route;
        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        TagData tagData(
            flightplan,
            radarTarget,
            1,
            EuroScopePlugIn::TAG_DATA_CORRELATED,
            itemString,
            &euroscopeColourCode,
            &tagColour,
            &fontSize);

        EXPECT_CALL(flightplan, GetExtractedRoute()).Times(2).WillRepeatedly(Return(ByRef(route)));

        ON_CALL(flightplan, GetCallsign).WillByDefault(Return("BAW123"));

        EXPECT_CALL(flightplan, GetOrigin()).Times(1).WillOnce(Return("EGKK"));

        EXPECT_CALL(flightplan, GetDestination()).Times(1).WillOnce(Return("EGLL"));

        EXPECT_CALL(flightplan, GetCruiseLevel()).Times(1).WillOnce(Return(8000));

        std::shared_ptr<UKControllerPlugin::Integration::MessageInterface> expectedMessage =
            std::make_shared<IntentionCodeUpdatedMessage>("BAW123", "", "LL");

        EXPECT_CALL(this->mockIntegration, SendEvent(MatchMessageInterface(expectedMessage))).Times(1);

        handler->SetTagItemData(tagData);
        EXPECT_EQ("LL", tagData.GetItemString());
        handler->SetTagItemData(tagData);
        EXPECT_EQ("LL", tagData.GetItemString());
    }

    // These two tests proves that the cache drops a code if it becomes invalid.
    TEST_F(IntentionCodeEventHandlerTest, FlightplanEventClearsCache)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> route;
        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        TagData tagData(
            flightplan,
            radarTarget,
            1,
            EuroScopePlugIn::TAG_DATA_CORRELATED,
            itemString,
            &euroscopeColourCode,
            &tagColour,
            &fontSize);

        EXPECT_CALL(flightplan, GetExtractedRoute()).Times(2).WillRepeatedly(Return(ByRef(route)));

        ON_CALL(flightplan, GetCallsign).WillByDefault(Return("BAW123"));

        EXPECT_CALL(flightplan, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

        EXPECT_CALL(flightplan, GetDestination()).Times(2).WillOnce(Return("EGLL")).WillOnce(Return("EGSS"));

        EXPECT_CALL(flightplan, GetCruiseLevel()).Times(2).WillRepeatedly(Return(8000));

        handler->SetTagItemData(tagData);
        EXPECT_EQ("LL", tagData.GetItemString());
        this->handler->FlightPlanEvent(flightplan, radarTarget);
        handler->SetTagItemData(tagData);
        EXPECT_EQ("SS", tagData.GetItemString());
    }

    TEST_F(IntentionCodeEventHandlerTest, FlightplanDisconnectEventClearsCache)
    {
        NiceMock<MockEuroscopeExtractedRouteInterface> route;
        NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
        NiceMock<MockEuroScopeCRadarTargetInterface> radarTarget;
        TagData tagData(
            flightplan,
            radarTarget,
            1,
            EuroScopePlugIn::TAG_DATA_CORRELATED,
            itemString,
            &euroscopeColourCode,
            &tagColour,
            &fontSize);

        EXPECT_CALL(flightplan, GetExtractedRoute()).Times(2).WillRepeatedly(Return(ByRef(route)));

        ON_CALL(flightplan, GetCallsign).WillByDefault(Return("BAW123"));

        EXPECT_CALL(flightplan, GetOrigin()).Times(2).WillRepeatedly(Return("EGKK"));

        EXPECT_CALL(flightplan, GetDestination()).Times(2).WillOnce(Return("EGLL")).WillOnce(Return("EGSS"));

        EXPECT_CALL(flightplan, GetCruiseLevel()).Times(2).WillRepeatedly(Return(8000));

        handler->SetTagItemData(tagData);
        EXPECT_EQ("LL", tagData.GetItemString());
        handler->FlightPlanDisconnectEvent(flightplan);
        handler->SetTagItemData(tagData);
        EXPECT_EQ("SS", tagData.GetItemString());
    }

    TEST_F(IntentionCodeEventHandlerTest, ControllerSelfDisconnectClearsCache)
    {
        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        this->handler->SelfDisconnectEvent();
        EXPECT_EQ(0, this->handler->GetCache().TotalCached());
    }

    TEST_F(IntentionCodeEventHandlerTest, ControllerChangingCallsignClearsCache)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return("LON_S_CTR"));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(true));

        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ(0, this->handler->GetCache().TotalCached());
    }

    TEST_F(IntentionCodeEventHandlerTest, ControllerChangingCallsignSetsGeneratorsCallsign)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return("LON_S_CTR"));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(true));

        EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ("LON_S_CTR", this->handler->GetGenerator().GetUserControllerPosition());
    }

    TEST_F(IntentionCodeEventHandlerTest, CacheIsNotClearedIfControllerCallsignNotChanged)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return(""));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(true));

        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
    }

    TEST_F(IntentionCodeEventHandlerTest, GeneratorCallsignIsNotChangedIfControllerCallsignNotChanged)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return(""));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(true));

        EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
    }

    TEST_F(IntentionCodeEventHandlerTest, CacheIsNotClearedIfControllerIsNotUser)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return("LON_S_CTR"));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(false));

        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ(1, this->handler->GetCache().TotalCached());
    }

    TEST_F(IntentionCodeEventHandlerTest, GeneratorCallsignIsNotChangedIfControllerIsNotUser)
    {
        NiceMock<MockEuroScopeCControllerInterface> controller;
        ON_CALL(controller, GetCallsign()).WillByDefault(Return("LON_S_CTR"));
        ON_CALL(controller, IsCurrentUser()).WillByDefault(Return(false));

        EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
        this->handler->ControllerUpdateEvent(controller);
        EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
    }
} // namespace UKControllerPluginTest::IntentionCode
