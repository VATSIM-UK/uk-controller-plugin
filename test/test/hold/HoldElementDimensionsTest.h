#include "pch/pch.h"
#include "hold/HoldElementDimensions.h"

using UKControllerPlugin::Hold::HoldElementDimensions;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldElementDimensionsTest : public Test
        {
            public:
                HoldElementDimensions compare = { 50, 60, 150, 300 };
        };

        TEST_F(HoldElementDimensionsTest, EqualityReturnsTrueAllEqual)
        {
            HoldElementDimensions dimensions = { 50, 60, 150, 300 };
            EXPECT_TRUE(dimensions == this->compare);
        }

        TEST_F(HoldElementDimensionsTest, EqualityReturnsFalseTopLeftDifferent)
        {
            HoldElementDimensions dimensions = { 40, 60, 150, 300 };
            EXPECT_FALSE(dimensions == this->compare);
        }

        TEST_F(HoldElementDimensionsTest, EqualityReturnsFalseTopRightDifferent)
        {
            HoldElementDimensions dimensions = { 50, 70, 150, 300 };
            EXPECT_FALSE(dimensions == this->compare);
        }

        TEST_F(HoldElementDimensionsTest, EqualityReturnsFalseWidthDifferent)
        {
            HoldElementDimensions dimensions = { 50, 60, 160, 300 };
            EXPECT_FALSE(dimensions == this->compare);
        }

        TEST_F(HoldElementDimensionsTest, EqualityReturnsFalseHeightDifferent)
        {
            HoldElementDimensions dimensions = { 50, 60, 150, 310 };
            EXPECT_FALSE(dimensions == this->compare);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
