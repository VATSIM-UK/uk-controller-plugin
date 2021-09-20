#include "hold/HoldingAircraft.h"
#include "hold/CompareHoldingAircraft.h"

using ::testing::Test;
using UKControllerPlugin::Hold::CompareHoldingAircraft;
using UKControllerPlugin::Hold::HoldingAircraft;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldingAircraftTest : public Test
        {
            public:
            CompareHoldingAircraft compare;
        };

        TEST_F(CompareHoldingAircraftTest, LessThanStringReturnsTrueIfLessThan)
        {
            std::shared_ptr<HoldingAircraft> aircraft = std::make_shared<HoldingAircraft>("BAW123", "BNN");
            EXPECT_TRUE(compare(aircraft, "BAW124"));
        }

        TEST_F(CompareHoldingAircraftTest, LessThanStructReturnsTrueIfLessThan)
        {
            std::shared_ptr<HoldingAircraft> aircraft = std::make_shared<HoldingAircraft>("BAW124", "BNN");
            EXPECT_TRUE(compare("BAW123", aircraft));
        }

        TEST_F(CompareHoldingAircraftTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            std::shared_ptr<HoldingAircraft> aircraft1 = std::make_shared<HoldingAircraft>("BAW123", "BNN");
            std::shared_ptr<HoldingAircraft> aircraft2 = std::make_shared<HoldingAircraft>("BAW124", "BNN");
            EXPECT_TRUE(compare(aircraft1, aircraft2));
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
