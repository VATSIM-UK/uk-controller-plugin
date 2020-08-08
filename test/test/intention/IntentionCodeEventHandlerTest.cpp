#include "pch/pch.h"
#include "intention/IntentionCodeEventHandler.h"
#include "intention/IntentionCodeGenerator.h"
#include "intention/IntentionCodeFactory.h"
#include "intention/IntentionCodeCache.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "mock/MockEuroScopeCControllerInterface.h"
#include "mock/MockEuroscopeExtractedRouteInterface.h"
#include "intention/SectorExitRepositoryFactory.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroScopeCRadarTargetInterface.h"
#include "tag/TagData.h"

using UKControllerPlugin::Tag::TagData;
using UKControllerPlugin::IntentionCode::IntentionCodeGenerator;
using UKControllerPlugin::IntentionCode::IntentionCodeEventHandler;
using UKControllerPlugin::IntentionCode::IntentionCodeFactory;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCControllerInterface;
using UKControllerPlugin::IntentionCode::SectorExitRepositoryFactory;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;
using UKControllerPluginTest::Euroscope::MockEuroScopeCRadarTargetInterface;
using ::testing::StrictMock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace IntentionCode {

        class IntentionCodeEventHandlerTest : public Test
        {
            public:
                void SetUp()
                {
                    IntentionCodeCache cache;
                    cache.RegisterAircraft("BAW456", {"KK", false, -1});
                    PersistenceContainer container;
                    this->handler = std::unique_ptr<IntentionCodeEventHandler>(new IntentionCodeEventHandler(
                        std::move(*IntentionCodeFactory::Create(std::move(*SectorExitRepositoryFactory::Create()))),
                        cache
                    ));
                };

                double fontSize = 24.1;
                COLORREF tagColour = RGB(255, 255, 255);
                int euroscopeColourCode = EuroScopePlugIn::TAG_COLOR_ASSUMED;
                char itemString[16] = "Foooooo";
                std::unique_ptr<IntentionCodeEventHandler> handler;
        };

        TEST_F(IntentionCodeEventHandlerTest, GetTagItemDescriptionReturnsCorrectDescription)
        {
            EXPECT_TRUE("UKCP Intention Code" == this->handler->GetTagItemDescription());
        }

        TEST_F(IntentionCodeEventHandlerTest, GetTagItemDataGeneratesIntentionCodeIfNonePresent)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> route;
            StrictMock<MockEuroScopeCFlightPlanInterface> flightplan;
            StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
            TagData tagData(
                flightplan,
                radarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            EXPECT_CALL(flightplan, GetExtractedRoute())
                .Times(1)
                .WillOnce(Return(ByRef(route)));

            EXPECT_CALL(flightplan, GetCallsign())
                .Times(3)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(flightplan, GetOrigin())
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(flightplan, GetDestination())
                .Times(1)
                .WillOnce(Return("EGLL"));

            EXPECT_CALL(flightplan, GetCruiseLevel())
                .Times(1)
                .WillOnce(Return(8000));

            handler->SetTagItemData(tagData);
            EXPECT_EQ("LL", tagData.GetItemString());
        }

        TEST_F(IntentionCodeEventHandlerTest, GetTagItemDataCachesCodeAndReusesIt)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> route;
            StrictMock<MockEuroScopeCFlightPlanInterface> flightplan;
            StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
            TagData tagData(
                flightplan,
                radarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            EXPECT_CALL(flightplan, GetExtractedRoute())
                .Times(2)
                .WillRepeatedly(Return(ByRef(route)));

            EXPECT_CALL(flightplan, GetCallsign())
                .Times(6)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(flightplan, GetOrigin())
                .Times(1)
                .WillOnce(Return("EGKK"));

            EXPECT_CALL(flightplan, GetDestination())
                .Times(1)
                .WillOnce(Return("EGLL"));

            EXPECT_CALL(flightplan, GetCruiseLevel())
                .Times(1)
                .WillOnce(Return(8000));

            handler->SetTagItemData(tagData);
            EXPECT_EQ("LL", tagData.GetItemString());
            handler->SetTagItemData(tagData);
            EXPECT_EQ("LL", tagData.GetItemString());
        }

        // These two tests proves that the cache drops a code if it becomes invalid.
        TEST_F(IntentionCodeEventHandlerTest, FlightplanEventClearsCache)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> route;
            StrictMock<MockEuroScopeCFlightPlanInterface> flightplan;
            StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
            TagData tagData(
                flightplan,
                radarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            EXPECT_CALL(flightplan, GetExtractedRoute())
                .Times(2)
                .WillRepeatedly(Return(ByRef(route)));

            EXPECT_CALL(flightplan, GetCallsign())
                .Times(7)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(flightplan, GetOrigin())
                .Times(2)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(flightplan, GetDestination())
                .Times(2)
                .WillOnce(Return("EGLL"))
                .WillOnce(Return("EGSS"));

            EXPECT_CALL(flightplan, GetCruiseLevel())
                .Times(2)
                .WillRepeatedly(Return(8000));

            handler->SetTagItemData(tagData);
            EXPECT_EQ("LL", tagData.GetItemString());
            this->handler->FlightPlanEvent(flightplan, radarTarget);
            handler->SetTagItemData(tagData);
            EXPECT_EQ("SS", tagData.GetItemString());
        }

        TEST_F(IntentionCodeEventHandlerTest, FlightplanDisconnectEventClearsCache)
        {
            StrictMock<MockEuroscopeExtractedRouteInterface> route;
            StrictMock<MockEuroScopeCFlightPlanInterface> flightplan;
            StrictMock<MockEuroScopeCRadarTargetInterface> radarTarget;
            TagData tagData(
                flightplan,
                radarTarget,
                1,
                EuroScopePlugIn::TAG_DATA_CORRELATED,
                itemString,
                &euroscopeColourCode,
                &tagColour,
                &fontSize
            );

            EXPECT_CALL(flightplan, GetExtractedRoute())
                .Times(2)
                .WillRepeatedly(Return(ByRef(route)));

            EXPECT_CALL(flightplan, GetCallsign())
                .Times(7)
                .WillRepeatedly(Return("BAW123"));

            EXPECT_CALL(flightplan, GetOrigin())
                .Times(2)
                .WillRepeatedly(Return("EGKK"));

            EXPECT_CALL(flightplan, GetDestination())
                .Times(2)
                .WillOnce(Return("EGLL"))
                .WillOnce(Return("EGSS"));

            EXPECT_CALL(flightplan, GetCruiseLevel())
                .Times(2)
                .WillRepeatedly(Return(8000));

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
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(true));

            EXPECT_EQ(1, this->handler->GetCache().TotalCached());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ(0, this->handler->GetCache().TotalCached());
        }

        TEST_F(IntentionCodeEventHandlerTest, ControllerChangingCallsignSetsGeneratorsCallsign)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(true));

            EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ("LON_S_CTR", this->handler->GetGenerator().GetUserControllerPosition());
        }

        TEST_F(IntentionCodeEventHandlerTest, CacheIsNotClearedIfControllerCallsignNotChanged)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return(""));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(true));


            EXPECT_EQ(1, this->handler->GetCache().TotalCached());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        }

        TEST_F(IntentionCodeEventHandlerTest, GeneratorCallsignIsNotChangedIfControllerCallsignNotChanged)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return(""));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(true));

            EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
        }

        TEST_F(IntentionCodeEventHandlerTest, CacheIsNotClearedIfControllerIsNotUser)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(false));


            EXPECT_EQ(1, this->handler->GetCache().TotalCached());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ(1, this->handler->GetCache().TotalCached());
        }

        TEST_F(IntentionCodeEventHandlerTest, GeneratorCallsignIsNotChangedIfControllerIsNotUser)
        {
            NiceMock<MockEuroScopeCControllerInterface> controller;
            ON_CALL(controller, GetCallsign())
                .WillByDefault(Return("LON_S_CTR"));
            ON_CALL(controller, IsCurrentUser())
                .WillByDefault(Return(false));

            EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
            this->handler->ControllerUpdateEvent(controller);
            EXPECT_EQ("", this->handler->GetGenerator().GetUserControllerPosition());
        }
    }  // namespace IntentionCode
}  // namespace UKControllerPluginTest
