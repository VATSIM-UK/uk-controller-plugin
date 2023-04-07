#include "handoff/ClearCacheOnRunwayDialogSave.h"
#include "handoff/HandoffCache.h"
#include "handoff/ResolvedHandoff.h"

using UKControllerPlugin::Handoff::ClearCacheOnRunwayDialogSave;
using UKControllerPlugin::Handoff::HandoffCache;
using UKControllerPlugin::Handoff::ResolvedHandoff;

namespace UKControllerPluginTest::Handoff {

    class ClearCacheOnRunwayDialogSaveTest : public testing::Test
    {
    };

    TEST_F(ClearCacheOnRunwayDialogSaveTest, FlushingCallsignsClearsCache)
    {
        auto cache = std::make_shared<HandoffCache>();
        ClearCacheOnRunwayDialogSave clear(cache);

        cache->Add(std::make_shared<ResolvedHandoff>("BAW123", nullptr, nullptr, nullptr));
        cache->Add(std::make_shared<ResolvedHandoff>("BAW456", nullptr, nullptr, nullptr));
        cache->Add(std::make_shared<ResolvedHandoff>("BAW789", nullptr, nullptr, nullptr));

        EXPECT_EQ(3, cache->Count());
        clear.RunwayDialogSaved();
        EXPECT_EQ(0, cache->Count());
    }
} // namespace UKControllerPluginTest::Handoff
