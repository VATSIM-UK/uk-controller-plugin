#include "handoff/HandoffCache.h"
#include "handoff/ResolvedHandoff.h"

using UKControllerPlugin::Handoff::HandoffCache;
using UKControllerPlugin::Handoff::ResolvedHandoff;

namespace UKControllerPluginTest::Handoff {
    class HandoffCacheTest : public testing::Test
    {
        public:
        HandoffCacheTest()
            : handoff1(std::make_shared<ResolvedHandoff>("BAW123", 122.800)),
              handoff2(std::make_shared<ResolvedHandoff>("BAW456", 122.800))
        {
        }

        std::shared_ptr<ResolvedHandoff> handoff1;
        std::shared_ptr<ResolvedHandoff> handoff2;
        HandoffCache cache;
    };

    TEST_F(HandoffCacheTest, ItStartsEmpty)
    {
        EXPECT_EQ(0, cache.Count());
    }

    TEST_F(HandoffCacheTest, ItAddsResolvedHandoffs)
    {
        cache.Add(handoff1);
        cache.Add(handoff2);

        EXPECT_EQ(2, cache.Count());
        EXPECT_EQ(handoff1, cache.Get("BAW123"));
        EXPECT_EQ(handoff2, cache.Get("BAW456"));
    }

    TEST_F(HandoffCacheTest, ItDoesntAddDuplicateCallsigns)
    {
        cache.Add(handoff1);
        cache.Add(handoff1);
        cache.Add(handoff1);
        cache.Add(handoff2);
        cache.Add(handoff2);
        cache.Add(handoff2);

        EXPECT_EQ(2, cache.Count());
    }

    TEST_F(HandoffCacheTest, ItReturnsNullptrOnHandoffNotFound)
    {
        cache.Add(handoff1);
        cache.Add(handoff2);

        EXPECT_EQ(nullptr, cache.Get("BAW999"));
    }

    TEST_F(HandoffCacheTest, ItRemovesHandoffs)
    {
        cache.Add(handoff1);
        cache.Add(handoff2);
        cache.Delete("BAW123");

        EXPECT_EQ(1, cache.Count());
        EXPECT_EQ(nullptr, cache.Get("BAW123"));
        EXPECT_EQ(handoff2, cache.Get("BAW456"));
    }

    TEST_F(HandoffCacheTest, ItHandlesNonExistantHandoffRemoval)
    {
        EXPECT_NO_THROW(cache.Delete("BAW123"));
    }
} // namespace UKControllerPluginTest::Handoff
