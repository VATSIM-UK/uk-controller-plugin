#include "pch/pch.h"
#include "hold/BlockedHoldLevelRestriction.h"

using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class BlockedHoldLevelRestrictionTest : public Test
        {
            public:
                BlockedHoldLevelRestrictionTest()
                    : restriction({7000, 13000})
                {

                }
                
                BlockedHoldLevelRestriction restriction;
        };

        TEST_F(BlockedHoldLevelRestrictionTest, IsLevelRestrictedReturnsTrueIfLevelInSet)
        {
            EXPECT_TRUE(this->restriction.LevelRestricted(13000));
        }


        TEST_F(BlockedHoldLevelRestrictionTest, IsLevelRestrictedReturnsFalseLevelNotInSet)
        {
            EXPECT_FALSE(this->restriction.LevelRestricted(8000));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest