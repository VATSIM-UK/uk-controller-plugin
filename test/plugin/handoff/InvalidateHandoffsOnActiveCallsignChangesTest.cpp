#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/InvalidateHandoffsOnActiveCallsignChanges.h"
#include "handoff/ResolvedHandoff.h"
#include "mock/MockDepartureHandoffResolver.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"
#include "mock/MockEuroscopePluginLoopbackInterface.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::ResolvedHandoff;

namespace UKControllerPluginTest::Handoff {
    class InvalidateHandoffsOnActiveCallsignChangesTest : public testing::Test
    {
        public:
        InvalidateHandoffsOnActiveCallsignChangesTest()
            : position1(std::make_shared<ControllerPosition>(
                  1, "LON_S_CTR", 129.420, std::vector<std::string>{}, true, false)),
              position2(std::make_shared<ControllerPosition>(
                  2, "LON_SC_CTR", 132.6, std::vector<std::string>{}, true, false)),
              position3(
                  std::make_shared<ControllerPosition>(-1, "UNICOM", 122.800, std::vector<std::string>{}, true, false)),
              hierarchy1(std::make_shared<ControllerPositionHierarchy>()),
              hierarchy2(std::make_shared<ControllerPositionHierarchy>()),
              hierarchy3(std::make_shared<ControllerPositionHierarchy>()),
              hierarchy4(std::make_shared<ControllerPositionHierarchy>()),
              callsign("LON_S_CTR", "Test", *position1, true),
              mockResolver(std::make_shared<testing::NiceMock<MockDepartureHandoffResolver>>()),
              changes(mockResolver, mockPlugin)
        {
            hierarchy1->AddPosition(position1);
            hierarchy1->AddPosition(position2);
            hierarchy2->AddPosition(position2);
            hierarchy2->AddPosition(position1);
            hierarchy4->AddPosition(position2);
        }

        std::shared_ptr<ControllerPosition> position1;
        std::shared_ptr<ControllerPosition> position2;
        std::shared_ptr<ControllerPosition> position3;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy1;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy2;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy3;
        std::shared_ptr<ControllerPositionHierarchy> hierarchy4;
        ActiveCallsign callsign;
        testing::NiceMock<Euroscope::MockEuroscopePluginLoopbackInterface> mockPlugin;
        std::shared_ptr<testing::NiceMock<MockDepartureHandoffResolver>> mockResolver;
        UKControllerPlugin::Handoff::InvalidateHandoffsOnActiveCallsignChanges changes;
    };

    TEST_F(InvalidateHandoffsOnActiveCallsignChangesTest, FlushingCallsignsInvalidatesAllFlightplans)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        auto mockFlightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        auto mockFlightplan3 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget3 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});
        mockPlugin.AddAllFlightplansItem({mockFlightplan2, mockRadarTarget2});
        mockPlugin.AddAllFlightplansItem({mockFlightplan3, mockRadarTarget3});

        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan2))).Times(1);
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan3))).Times(1);

        changes.CallsignsFlushed();
    }

    TEST_F(InvalidateHandoffsOnActiveCallsignChangesTest, CallsignsLoggingOffEvictOnResolvedControllerMatch)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        auto mockFlightplan2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget2 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        auto mockFlightplan3 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget3 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();

        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});
        mockPlugin.AddAllFlightplansItem({mockFlightplan2, mockRadarTarget2});
        mockPlugin.AddAllFlightplansItem({mockFlightplan3, mockRadarTarget3});

        auto handoffFlightplan1 = std::make_shared<ResolvedHandoff>("BAW123", position1, nullptr, nullptr);
        auto handoffFlightplan2 = std::make_shared<ResolvedHandoff>("BAW456", position1, nullptr, nullptr);
        auto handoffFlightplan3 = std::make_shared<ResolvedHandoff>("BAW789", position2, nullptr, nullptr);

        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(handoffFlightplan1));
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan2))).Times(1);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan2)))
            .Times(2)
            .WillRepeatedly(testing::Return(handoffFlightplan2));
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan3))).Times(0);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan3)))
            .Times(1)
            .WillOnce(testing::Return(handoffFlightplan3));

        changes.ActiveCallsignRemoved(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignsLoggingOnInvalidatesAndResolvesIfControllerPreceedsInSidHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy1, hierarchy2);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);

        // Resolved to LON_SC, LON_S comes on and precedes in SID hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignsLoggingOnDoesntInvalidateAndResolveIfControllerAfterInSidHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy2, hierarchy1);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1))).WillOnce(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(0);

        // Resolved to LON_SC, LON_S comes on but is after SC in the hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnInvalidatesAndResolvesIfControllerPreceedsInAirfieldHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy3, hierarchy1);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);

        // Resolve to LON_SC, LON_S comes on and preceeds in hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnInvalidatesAndResolvesIfResolvedControllerInAirfieldHierarchyButControllerLoggingOnIsInSidHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy1, hierarchy4);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);

        // Resolve to LON_SC on the airfield hierarchy, LON_S comes on, is not in the airfield hierarchy but is
        // in the SID one, should clear.
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnDoesntInvalidateAndResolveIfControllerAfterInAirfieldHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy3, hierarchy2);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1))).WillOnce(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(0);

        // Resolve to LON_SC, LON_S comes on but is after SC in the hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnInvalidatesAndResolvesIfResolvedControllerNotInHierarchyButControllerInSidHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy1, hierarchy3);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);

        // Resolve to unicom, LON_S comes on and is in SID hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnInvalidatesAndResolvesIfResolvedControllerNotInHierarchyButControllerInAirfieldHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy3, hierarchy1);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1)))
            .Times(2)
            .WillRepeatedly(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(1);

        // Resolve to unicom, LON_S comes on and is in airfield hierarchy
        changes.ActiveCallsignAdded(callsign);
    }

    TEST_F(
        InvalidateHandoffsOnActiveCallsignChangesTest,
        CallsignLoggingOnDoesntInvalidateAndResolveIfResolvedControllerNotInHierarchyButControllerNotInHierarchy)
    {
        auto mockFlightplan1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCFlightPlanInterface>>();
        auto mockRadarTarget1 = std::make_shared<testing::NiceMock<Euroscope::MockEuroScopeCRadarTargetInterface>>();
        mockPlugin.AddAllFlightplansItem({mockFlightplan1, mockRadarTarget1});

        // Make sure we resolve and get the handoff first time
        auto resolvedHandoff = std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy3, hierarchy3);
        EXPECT_CALL(*mockResolver, Resolve(testing::Ref(*mockFlightplan1))).WillOnce(testing::Return(resolvedHandoff));

        // Subsequent times, we should invalidate and resolve
        EXPECT_CALL(*mockResolver, Invalidate(testing::Ref(*mockFlightplan1))).Times(0);

        // Resolve to unicom, LON_S comes on and is in neither hierarchy
        changes.ActiveCallsignAdded(callsign);
    }
} // namespace UKControllerPluginTest::Handoff
