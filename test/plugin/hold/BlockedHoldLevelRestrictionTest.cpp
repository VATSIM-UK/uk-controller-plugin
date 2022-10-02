#include "hold/BlockedHoldLevelRestriction.h"

using ::testing::Test;
using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;

namespace UKControllerPluginTest {
    namespace Hold {

        class BlockedHoldLevelRestrictionTest : public Test
        {
            public:
            BlockedHoldLevelRestrictionTest() : restriction({7000, 13000})
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

        TEST_F(BlockedHoldLevelRestrictionTest, ItCanReturnBlockedLevels)
        {
            std::set<unsigned int> expected = {7000, 13000};
            EXPECT_EQ(expected, this->restriction.GetLevels());
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
