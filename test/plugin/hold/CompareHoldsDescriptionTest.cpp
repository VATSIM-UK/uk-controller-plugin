#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/HoldingData.h"
#include "hold/CompareHoldsDescription.h"

using ::testing::Test;
using UKControllerPlugin::Hold::CompareHoldsDescription;
using UKControllerPlugin::Hold::HoldingData;

namespace UKControllerPluginTest {
    namespace Hold {

        class CompareHoldsDescriptionTest : public Test
        {
            public:
            CompareHoldsDescription compare;
        };

        TEST_F(CompareHoldsDescriptionTest, LessThanStringReturnsTrueIfLessThan)
        {
            HoldingData hold = {1, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {}};
            EXPECT_TRUE(compare(hold, "WILLO"));
        }

        TEST_F(CompareHoldsDescriptionTest, LessThanStructReturnsTrueIfLessThan)
        {
            HoldingData hold = {1, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {}};
            EXPECT_TRUE(compare("ROSUN", hold));
        }

        TEST_F(CompareHoldsDescriptionTest, CompareReturnsTrueIfFirstLessThanLast)
        {
            HoldingData hold1 = {1, "ROSUN", "ROSUN", 6000, 15000, 50, "left", {}};
            HoldingData hold2 = {2, "TIMBA", "TIMBA", 6000, 15000, 50, "left", {}};
            EXPECT_TRUE(compare(hold1, hold2));
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
