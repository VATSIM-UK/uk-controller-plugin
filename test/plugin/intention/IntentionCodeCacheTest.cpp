#include "intention/IntentionCodeCache.h"
#include "intention/IntentionCodeData.h"

using ::testing::Return;
using ::testing::StrictMock;
using UKControllerPlugin::IntentionCode::IntentionCodeCache;
using UKControllerPlugin::IntentionCode::IntentionCodeData;
using UKControllerPluginTest::Euroscope::MockEuroscopeExtractedRouteInterface;

namespace UKControllerPluginTest::IntentionCode {

    TEST(IntentionCodeCache, GetIntentionCodeForCallsignReturnsDefaultIfNoneFound)
    {
        IntentionCodeCache cache;
        EXPECT_TRUE("--" == cache.GetIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, GetIntentionCodeForCallsignReturnsCodeCorrectly)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("C2", 1, 1, "REDFA", ""));
        EXPECT_TRUE("C2" == cache.GetIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, StartsEmpty)
    {
        IntentionCodeCache cache;
        EXPECT_EQ(0, cache.TotalCached());
    }

    TEST(IntentionCodeCache, HasIntentionCodeForAircraftReturnsFalseIfNoneFound)
    {
        IntentionCodeCache cache;
        EXPECT_FALSE(cache.HasIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, RegisterAircraftAddsAircraftToCache)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        EXPECT_TRUE(cache.HasIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, RegisterAircraftDoesNotDuplicateAircraft)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.RegisterAircraft("BAW123", IntentionCodeData("D2", 1, 1, "KOK", ""));
        EXPECT_EQ(1, cache.TotalCached());
    }

    TEST(IntentionCodeCache, RegisterAircraftDoesNotChangeExistingCodes)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.RegisterAircraft("BAW123", IntentionCodeData("D2", 1, 1, "KOK", ""));
        EXPECT_TRUE("D1" == cache.GetIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, UnregisterAircraftRemovesAircraftFromCache)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.UnregisterAircraft("BAW123");
        EXPECT_FALSE(cache.HasIntentionCodeForAircraft("BAW123"));
    }

    TEST(IntentionCodeCache, UnregisterAircraftDoesNothingIfDoesntExist)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.UnregisterAircraft("BAW456");
        EXPECT_TRUE(cache.HasIntentionCodeForAircraft("BAW123"));
        EXPECT_EQ(1, cache.TotalCached());
    }

    TEST(IntentionCodeCache, ClearCacheEmptiesCache)
    {
        IntentionCodeCache cache;
        cache.RegisterAircraft("BAW123", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.RegisterAircraft("BAW456", IntentionCodeData("D1", 1, 1, "KOK", ""));
        cache.Clear();
        EXPECT_FALSE(cache.HasIntentionCodeForAircraft("BAW123"));
        EXPECT_FALSE(cache.HasIntentionCodeForAircraft("BAW456"));
        EXPECT_EQ(0, cache.TotalCached());
    }

    TEST(IntentionCodeCache, ItReturnsDataForAircraftIfSet)
    {
        IntentionCodeCache cache;

        cache.RegisterAircraft("BAW123", IntentionCodeData("C2", 5, 6, "REDFA", "SOMVA"));
        auto data = cache.GetDataForAircraft("BAW123");
        EXPECT_EQ("C2", data.intentionCode);
        EXPECT_EQ(5, data.exitPointIndex);
        EXPECT_EQ(6, data.ukExitPointIndex);
        EXPECT_EQ("REDFA", data.exitPoint);
        EXPECT_EQ("SOMVA", data.ukExitPoint);
    }

    TEST(IntentionCodeCache, ItReturnsDefaultDataForAircraftIfNotSet)
    {
        IntentionCodeCache cache;

        cache.RegisterAircraft("BAW123", IntentionCodeData("C2", 5, 6, "REDFA", "SOMVA"));
        auto data = cache.GetDataForAircraft("BAW456");
        EXPECT_EQ(IntentionCodeData::INVALID_EXIT_POINT, data.exitPointIndex);
    }
} // namespace UKControllerPluginTest::IntentionCode
