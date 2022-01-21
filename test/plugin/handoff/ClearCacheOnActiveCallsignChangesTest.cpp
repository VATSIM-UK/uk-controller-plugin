#include "controller/ActiveCallsign.h"
#include "controller/ControllerPosition.h"
#include "controller/ControllerPositionHierarchy.h"
#include "handoff/ClearCacheOnActiveCallsignChanges.h"
#include "handoff/HandoffCache.h"
#include "handoff/ResolvedHandoff.h"

using UKControllerPlugin::Controller::ActiveCallsign;
using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionHierarchy;
using UKControllerPlugin::Handoff::ClearCacheOnActiveCallsignChanges;
using UKControllerPlugin::Handoff::HandoffCache;
using UKControllerPlugin::Handoff::ResolvedHandoff;

namespace UKControllerPluginTest::Handoff {
    class ClearCacheOnActiveCallsignChangesTest : public testing::Test
    {
        public:
        ClearCacheOnActiveCallsignChangesTest()
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
              callsign("LON_S_CTR", "Test", *position1, true), changes(cache)
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
        HandoffCache cache;
        ClearCacheOnActiveCallsignChanges changes;
    };

    TEST_F(ClearCacheOnActiveCallsignChangesTest, FlushingCallsignsClearsCache)
    {
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", nullptr, nullptr, nullptr));
        cache.Add(std::make_shared<ResolvedHandoff>("BAW456", nullptr, nullptr, nullptr));
        changes.CallsignsFlushed();
        EXPECT_EQ(0, cache.Count());
    }

    TEST_F(ClearCacheOnActiveCallsignChangesTest, CallsignsLoggingOffEvictOnResolvedControllerMatch)
    {
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position1, nullptr, nullptr));
        cache.Add(std::make_shared<ResolvedHandoff>("BAW456", position1, nullptr, nullptr));
        cache.Add(std::make_shared<ResolvedHandoff>("BAW789", position2, nullptr, nullptr));
        changes.ActiveCallsignRemoved(callsign);
        EXPECT_EQ(1, cache.Count());
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
        EXPECT_EQ(nullptr, cache.Get("BAW456"));
        EXPECT_NE(nullptr, cache.Get("BAW789"));
    }

    TEST_F(ClearCacheOnActiveCallsignChangesTest, CallsignsLoggingOnClearsCacheIfControllerPreceedsInSidHierarchy)
    {
        // Resolved to LON_SC, LON_S comes on and precedes in SID hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy1, hierarchy2));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
    }

    TEST_F(ClearCacheOnActiveCallsignChangesTest, CallsignsLoggingOnDoesntClearCacheIfControllerAfterInSidHierarchy)
    {
        // Resolved to LON_SC, LON_S comes on but is after SC in the hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy2, hierarchy1));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_NE(nullptr, cache.Get("BAW123"));
    }

    TEST_F(ClearCacheOnActiveCallsignChangesTest, CallsignLoggingOnClearsCacheIfControllerPreceedsInAirfieldHierarchy)
    {
        // Resolve to LON_SC, LON_S comes on and preceeds in hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy3, hierarchy1));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
    }

    TEST_F(
        ClearCacheOnActiveCallsignChangesTest,
        CallsignLoggingOnClearsCacheIfResolvedControllerInAirfieldHierarchyButControllerLoggingOnIsInSidHierarchy)
    {
        // Resolve to LON_SC on the airfield hierarchy, LON_S comes on, is not in the airfield hierarchy but is
        // in the SID one, should clear.
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy1, hierarchy4));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
    }

    TEST_F(ClearCacheOnActiveCallsignChangesTest, CallsignLoggingOnDoesntClearCacheIfControllerAfterInAirfieldHierarchy)
    {
        // Resolve to LON_SC, LON_S comes on but is after SC in the hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position2, hierarchy3, hierarchy2));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_NE(nullptr, cache.Get("BAW123"));
    }

    TEST_F(
        ClearCacheOnActiveCallsignChangesTest,
        CallsignLoggingOnClearsCacheIfResolvedControllerNotInHierarchyButControllerInSidHierarchy)
    {
        // Resolve to unicom, LON_S comes on and is in SID hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy1, hierarchy3));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
    }

    TEST_F(
        ClearCacheOnActiveCallsignChangesTest,
        CallsignLoggingOnClearsCacheIfResolvedControllerNotInHierarchyButControllerInAirfieldHierarchy)
    {
        // Resolve to unicom, LON_S comes on and is in airfield hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy3, hierarchy1));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
    }

    TEST_F(
        ClearCacheOnActiveCallsignChangesTest,
        CallsignLoggingOnDoesntClearCacheIfResolvedControllerNotInHierarchyButControllerNotInHierarchy)
    {
        // Resolve to unicom, LON_S comes on and is in neither hierarchy
        cache.Add(std::make_shared<ResolvedHandoff>("BAW123", position3, hierarchy3, hierarchy3));
        changes.ActiveCallsignAdded(callsign);
        EXPECT_NE(nullptr, cache.Get("BAW123"));
    }
} // namespace UKControllerPluginTest::Handoff
