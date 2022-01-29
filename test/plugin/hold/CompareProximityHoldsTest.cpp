#include "hold/CompareProximityHolds.h"
#include "hold/ProximityHold.h"
#include "time/SystemClock.h"

using UKControllerPlugin::Hold::CompareProximityHolds;
using UKControllerPlugin::Hold::ProximityHold;
using UKControllerPlugin::Time::TimeNow;

namespace UKControllerPluginTest::Hold {
    class CompareProximityHoldsTest : public testing::Test
    {
        public:
        CompareProximityHolds compare;
    };

    TEST_F(CompareProximityHoldsTest, LessThanStringReturnsTrueIfLessThan)
    {
        std::shared_ptr<ProximityHold> aircraft = std::make_shared<ProximityHold>("BAW123", "BNM", TimeNow());
        EXPECT_TRUE(compare(aircraft, "BNN"));
    }

    TEST_F(CompareProximityHoldsTest, LessThanStructReturnsTrueIfLessThan)
    {
        std::shared_ptr<ProximityHold> aircraft = std::make_shared<ProximityHold>("BAW124", "BNN", TimeNow());
        EXPECT_TRUE(compare("BNM", aircraft));
    }

    TEST_F(CompareProximityHoldsTest, CompareReturnsTrueIfFirstLessThanLast)
    {
        std::shared_ptr<ProximityHold> aircraft1 = std::make_shared<ProximityHold>("BAW123", "BNM", TimeNow());
        std::shared_ptr<ProximityHold> aircraft2 = std::make_shared<ProximityHold>("BAW124", "BNN", TimeNow());
        EXPECT_TRUE(compare(aircraft1, aircraft2));
    }
} // namespace UKControllerPluginTest::Hold
