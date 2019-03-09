#include "pch/pch.h"
#include "hold/BlockedHoldLevelRestriction.h"
#include "hold/BlockedHoldLevelRestrictionSerializer.h"

using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;
using UKControllerPlugin::Hold::ValidateLevelRestrictionData;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class BlockedHoldLevelRestrictionSerializerTest : public Test
        {
            public:
            BlockedHoldLevelRestrictionSerializerTest()
                : restriction(new BlockedHoldLevelRestriction({ 7000, 13000 }))
            {

            }

            std::unique_ptr<BlockedHoldLevelRestriction> restriction;
        };

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseNoKey)
        {
            nlohmann::json data;
            EXPECT_FALSE(ValidateLevelRestrictionData(data));
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseKeyNotArray)
        {
            nlohmann::json data;
            data["levels"] = "test";
            EXPECT_FALSE(ValidateLevelRestrictionData(data));
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseKeyEmptyArray)
        {
            nlohmann::json data;
            data["levels"] = {};
            EXPECT_FALSE(ValidateLevelRestrictionData(data));
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseLevelsNotNumber)
        {
            nlohmann::json data;
            data["levels"] = { "7000", 8000, 9000 };
            EXPECT_FALSE(ValidateLevelRestrictionData(data));
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsTrueValidData)
        {
            nlohmann::json data;
            data["levels"] = { 7000, 8000, 9000 };
            EXPECT_TRUE(ValidateLevelRestrictionData(data));
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, FromJsonReturnsNullptrInvalidData)
        {
            nlohmann::json data;
            EXPECT_EQ(nullptr, data.get<std::unique_ptr<BlockedHoldLevelRestriction>>());
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, FromJsonReturnsObjectIfValid)
        {
            nlohmann::json data;
            data["levels"] = { 7000, 8000, 9000 };

            std::set<unsigned int> expectedLevels;
            EXPECT_EQ(expectedLevels, data.get<std::unique_ptr<BlockedHoldLevelRestriction>>()->GetLevels());
        }

        TEST_F(BlockedHoldLevelRestrictionSerializerTest, ToJsonSerializesCorrectly)
        {
            nlohmann::json expected;
            expected["type"] = "level-block";
            expected["levels"] = { 7000, 13000 };

            EXPECT_EQ(expected, nlohmann::json(this->restriction));
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest