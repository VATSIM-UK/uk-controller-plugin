#include "hold/AbstractHoldLevelRestriction.h"
#include "hold/DeemedSeparatedHold.h"
#include "hold/DeemedSeparatedHoldSerializer.h"
#include "hold/HoldingData.h"
#include "bootstrap/PersistenceContainer.h"

using testing::Test;
using UKControllerPlugin::Hold::deemed_separated_from_json;
using UKControllerPlugin::Hold::DeemedSeparatedHold;
using UKControllerPlugin::Hold::ValidDeemedSeparatedData;

namespace UKControllerPluginTest {
    namespace Hold {
        class DeemedSeparatedHoldSerializerTest : public Test
        {
            public:
            nlohmann::json deemedSeparatedHold = {{"hold_id", 2}, {"vsl_insert_distance", 5}};

            std::set<std::unique_ptr<DeemedSeparatedHold>> deemedSeparatedHolds;
        };

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsFalseIfDataNotObject)
        {
            EXPECT_FALSE(ValidDeemedSeparatedData(nlohmann::json::array()));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsFalseIfHoldIdMissing)
        {
            deemedSeparatedHold.erase("hold_id");
            EXPECT_FALSE(ValidDeemedSeparatedData(deemedSeparatedHold));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsFalseIfHoldIdNotInteger)
        {
            deemedSeparatedHold["hold_id"] = "abc";
            EXPECT_FALSE(ValidDeemedSeparatedData(deemedSeparatedHold));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsFalseIfVslInsertDistanceMissing)
        {
            deemedSeparatedHold.erase("vsl_insert_distance");
            EXPECT_FALSE(ValidDeemedSeparatedData(deemedSeparatedHold));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsFalseIfVslInsertDistanceNotInteger)
        {
            deemedSeparatedHold["vsl_insert_distance"] = "abc";
            EXPECT_FALSE(ValidDeemedSeparatedData(deemedSeparatedHold));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestValidReturnsTrueIfValidData)
        {
            EXPECT_TRUE(ValidDeemedSeparatedData(deemedSeparatedHold));
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestItLoadsDeemedSeparated)
        {
            deemed_separated_from_json(nlohmann::json::array({deemedSeparatedHold}), deemedSeparatedHolds);
            EXPECT_EQ(1, deemedSeparatedHolds.size());
            EXPECT_EQ(2, (*deemedSeparatedHolds.cbegin())->identifier);
            EXPECT_EQ(5, (*deemedSeparatedHolds.cbegin())->vslInsertDistance);
        }

        TEST_F(DeemedSeparatedHoldSerializerTest, TestItDoesntLoadBadDeemedSeparated)
        {
            deemedSeparatedHold["vsl_insert_distance"] = "abc";
            deemed_separated_from_json(nlohmann::json::array({deemedSeparatedHold}), deemedSeparatedHolds);
            EXPECT_EQ(0, deemedSeparatedHolds.size());
        }
    } // namespace Hold
} // namespace UKControllerPluginTest
