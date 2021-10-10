#include "missedapproach/MissedApproach.h"
#include "missedapproach/MissedApproachCollection.h"
#include "missedapproach/RemoveExpiredMissedApproaches.h"
#include "time/SystemClock.h"

using UKControllerPlugin::MissedApproach::MissedApproach;
using UKControllerPlugin::MissedApproach::MissedApproachCollection;
using UKControllerPlugin::MissedApproach::RemoveExpiredMissedApproaches;
using UKControllerPlugin::Time::SetTestNow;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::MissedApproach {
    class RemoveExpiredMissedApproachesTest : public testing::Test
    {
        public:
        RemoveExpiredMissedApproachesTest()
            : collection(std::make_shared<MissedApproachCollection>()), remove(collection)
        {
            SetTestNow(std::chrono::system_clock::now());
        }

        std::shared_ptr<MissedApproachCollection> collection;
        RemoveExpiredMissedApproaches remove;
    };

    TEST_F(RemoveExpiredMissedApproachesTest, ItRemovesNothing)
    {
        EXPECT_NO_THROW(remove.TimedEventTrigger());
    }

    TEST_F(RemoveExpiredMissedApproachesTest, ItRemovesExpiredApproaches)
    {
        collection->Add(std::make_shared<class MissedApproach>(1, "BAW123", TimeNow() + std::chrono::seconds(5), true));
        collection->Add(std::make_shared<class MissedApproach>(2, "BAW456", TimeNow() - std::chrono::seconds(1), true));
        collection->Add(std::make_shared<class MissedApproach>(3, "BAW789", TimeNow(), true));

        remove.TimedEventTrigger();
        EXPECT_EQ(nullptr, collection->Get("BAW456"));
        EXPECT_NE(nullptr, collection->Get("BAW123"));
        EXPECT_NE(nullptr, collection->Get("BAW789"));
    }
} // namespace UKControllerPluginTest::MissedApproach
