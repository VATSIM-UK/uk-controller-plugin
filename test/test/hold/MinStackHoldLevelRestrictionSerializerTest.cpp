#include "pch/pch.h"
#include "hold/MinStackHoldLevelRestrictionSerializer.h"
#include "bootstrap/PersistenceContainer.h"

using UKControllerPlugin::Hold::RestrictionJsonValid;
using UKControllerPlugin::Hold::MinStackRestrictionFromJson;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using testing::Test;

namespace UKControllerPluginTest {
    namespace Hold {

        class MinStackHoldLevelRestrictionSerializerTest : public Test
        {
            public:
                PersistenceContainer container;
        };

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfLevelMissing)
        {
            nlohmann::json json = {
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfLevelNotString)
        {
            nlohmann::json json = {
                { "level", 123 },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfLevelNotValidString)
        {
            nlohmann::json json = {
                { "level", "MSL+66" },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfNoAirfieldTarget)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "override", 8000 }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfTargetNotString)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "target", 1 },
                { "override", 8000 }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsInvalidIfOverrideNotANumber)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "target", "EGCC" },
                { "override", "abc" }
            };

            EXPECT_FALSE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsValidWithNoOverride)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "target", "EGCC" },
            };

            EXPECT_TRUE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, JsonIsValidWithOverride)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_TRUE(RestrictionJsonValid(json));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, ItReturnsNullPtrOnInvalidJson)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "override", 8000 }
            };

            EXPECT_EQ(nullptr, MinStackRestrictionFromJson(json, this->container));
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, ItSetsTarget)
        {
            nlohmann::json json = {
                { "level", "MSL" },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_EQ("airfield.EGCC", MinStackRestrictionFromJson(json, this->container)->minStackKey);
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, ItSetsMslOffset)
        {
            nlohmann::json json = {
                { "level", "MSL+1" },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_EQ(1000, MinStackRestrictionFromJson(json, this->container)->minStackOffset);
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, ItSetsOverride)
        {
            nlohmann::json json = {
                { "level", "MSL+1" },
                { "target", "EGCC" },
                { "override", 8000 }
            };

            EXPECT_EQ(8000, MinStackRestrictionFromJson(json, this->container)->override);
        }

        TEST_F(MinStackHoldLevelRestrictionSerializerTest, ItSetsOverrideToZeroIfNoneProvided)
        {
            nlohmann::json json = {
                { "level", "MSL+1" },
                { "target", "EGCC" }
            };

            EXPECT_EQ(0, MinStackRestrictionFromJson(json, this->container)->override);
        }
    }  // namespace Hold
}  // namespace UKControllerPluginTest
