#include "pch/pch.h"

#include "bootstrap/PersistenceContainer.h"
#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/BlockedHoldLevelRestriction.h"
#include "hold/HoldRestrictionSerializer.h"
#include "hold/MinStackHoldLevelRestriction.h"
#include "minstack/MinStackManager.h"

using ::testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Hold::AbstractHoldLevelRestriction;
using UKControllerPlugin::Hold::BlockedHoldLevelRestriction;
using UKControllerPlugin::Hold::hold_restriction_from_json;
using UKControllerPlugin::Hold::MinStackHoldLevelRestriction;
using UKControllerPlugin::Hold::ValidRestrictionData;
using UKControllerPlugin::MinStack::MinStackManager;

namespace UKControllerPluginTest::Hold {

    class HoldRestrictionSerializerTest : public Test
    {
        public:
        HoldRestrictionSerializerTest()
        {
            container.minStack = std::make_shared<MinStackManager>();
        }

        PersistenceContainer container;
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
        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> actual;
        hold_restriction_from_json(data, actual, container);
        EXPECT_EQ(expected, actual);
    }

    TEST_F(HoldRestrictionSerializerTest, FromJsonIgnoresInvalidBlockTypes)
    {
        nlohmann::json data;
        data["type"] = "level-block";
        data["levels"] = "nope";

        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> expected = {};
        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> actual;
        hold_restriction_from_json(data, actual, container);
        EXPECT_EQ(expected, actual);
    }

    TEST_F(HoldRestrictionSerializerTest, FromJsonCreatesLevelBlockTypes)
    {
        nlohmann::json data = {{"type", "level-block"}, {"levels", {10000, 13000}}};
        nlohmann::json restrictions = nlohmann::json::array({data});

        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> actual;
        hold_restriction_from_json(restrictions, actual, container);

        EXPECT_EQ(1, actual.size());

        BlockedHoldLevelRestriction* actualCast = dynamic_cast<BlockedHoldLevelRestriction*>(actual.cbegin()->get());

        std::set<unsigned int> expectedLevels = {10000, 13000};
        EXPECT_EQ(expectedLevels, actualCast->GetLevels());
    }

    TEST_F(HoldRestrictionSerializerTest, FromJsonCreatesMinStackTypes)
    {
        nlohmann::json data = {{"type", "minimum-level"}, {"level", "MSL"}, {"target", "EGCC"}, {"override", 8000}};

        nlohmann::json restrictions = nlohmann::json::array({data});

        std::set<std::unique_ptr<AbstractHoldLevelRestriction>> actual;
        hold_restriction_from_json(restrictions, actual, container);

        EXPECT_EQ(1, actual.size());

        MinStackHoldLevelRestriction* actualCast = dynamic_cast<MinStackHoldLevelRestriction*>(actual.cbegin()->get());

        EXPECT_EQ(8000, actualCast->Override());
        EXPECT_EQ(0, actualCast->MinStackOffset());
        EXPECT_EQ("airfield.EGCC", actualCast->MinStackKey());
    }
} // namespace UKControllerPluginTest::Hold
