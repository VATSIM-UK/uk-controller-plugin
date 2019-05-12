#include "pch/pch.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/HoldRestrictionSerializer.h"
#include "hold/BlockedHoldLevelRestriction.h"

using UKControllerPlugin::Hold::AbstractHoldLevelRestriction;
using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;
using UKControllerPlugin::Hold::ValidRestrictionData;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class HoldRestrictionSerializerTest : public Test
        {
            public:
            HoldRestrictionSerializerTest()
            {

            }

        };

        TEST_F(HoldRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseNoKey)
        {
            nlohmann::json data;
            EXPECT_FALSE(ValidRestrictionData(data));
        }

        TEST_F(HoldRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseKeyNotArray)
        {
            nlohmann::json data;
            data["type"] = 123;
            EXPECT_FALSE(ValidRestrictionData(data));
        }

        TEST_F(HoldRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsFalseKeyNotValid)
        {
            nlohmann::json data;
            data["type"] = "not-valid";
            EXPECT_FALSE(ValidRestrictionData(data));
        }

        TEST_F(HoldRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsTrueBlockType)
        {
            nlohmann::json data;
            data["type"] = "level-block";
            EXPECT_TRUE(ValidRestrictionData(data));
        }

        TEST_F(HoldRestrictionSerializerTest, ValidateLevelRestrictionDataReturnsTrueMinimumLevelType)
        {
            nlohmann::json data;
            data["type"] = "minimum-level";
            EXPECT_TRUE(ValidRestrictionData(data));
        }

        TEST_F(HoldRestrictionSerializerTest, FromJsonIgnoresInvalidData)
        {
            nlohmann::json data;
            data["type"] = "lol";

            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> expected = {};
            EXPECT_EQ(expected, data.get<std::set<std::unique_ptr<AbstractHoldLevelRestriction>>>());
        }

        TEST_F(HoldRestrictionSerializerTest, FromJsonIgnoresInvalidBlockTypes)
        {
            nlohmann::json data;
            data["type"] = "level-block";
            data["levels"] = "nope";

            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> expected = {};
            EXPECT_EQ(expected, data.get<std::set<std::unique_ptr<AbstractHoldLevelRestriction>>>());
        }

        TEST_F(HoldRestrictionSerializerTest, FromJsonCreatesLevelBlockTypes)
        {
            nlohmann::json data = {
                { "type", "level-block" },
                { "levels", {10000, 13000} }
            };
            nlohmann::json restrictions = nlohmann::json::array({ data });

            std::set<std::unique_ptr<AbstractHoldLevelRestriction>> actual =
                restrictions.get<std::set<std::unique_ptr<AbstractHoldLevelRestriction>>>();
            EXPECT_EQ(1, actual.size());

            BlockedHoldLevelRestriction * actualCast =
                dynamic_cast<BlockedHoldLevelRestriction *>(actual.cbegin()->get());

            std::set<unsigned int> expectedLevels = { 10000, 13000 };
            EXPECT_EQ(expectedLevels, actualCast->GetLevels());
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
