#include "pch/pch.h"
#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"

using UKControllerPlugin::Hold::MinStackHoldLevelRestriction;
using UKControllerPlugin::MinStack::MinStackManager;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class MinStackHoldLevelRestrictionTest : public Test
        {
            public:
                MinStackHoldLevelRestrictionTest()
                    : restriction("EGLL", 1000, manager)
                {
                    manager.AddMsl("airfield.EGLL", "airfield", "Heathrow", 7000);
                }

                MinStackManager manager;
                MinStackHoldLevelRestriction restriction;
        };

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsTrueIfLevelBelowMinimum)
        {
            EXPECT_TRUE(this->restriction.LevelRestricted(7000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseIfLevelAtMinimum)
        {
            EXPECT_FALSE(this->restriction.LevelRestricted(8000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseIfLevelAboveMinimum)
        {
            EXPECT_FALSE(this->restriction.LevelRestricted(9000));
        }

        TEST_F(MinStackHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseNoMsl)
        {
            MinStackHoldLevelRestriction restriction2("EGKK", 1000, manager);
            EXPECT_FALSE(restriction2.LevelRestricted(8000));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
