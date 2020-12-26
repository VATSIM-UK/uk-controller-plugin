#include "pch/pch.h"
#include "wake/WakeCategoryMapper.h"

using UKControllerPlugin::Wake::WakeCategoryMapper;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Wake {
        class WakeCategoryMapperTest : public Test
        {
            public:
                WakeCategoryMapper mapper;
        };

        TEST_F(WakeCategoryMapperTest, TestItStartsEmpty)
        {
            EXPECT_EQ(0, this->mapper.Count());
        }

        TEST_F(WakeCategoryMapperTest, TestItAddsAMapping)
        {
            this->mapper.AddCategoryMapping("B733", "LM");
            EXPECT_EQ(1, this->mapper.Count());
        }

        TEST_F(WakeCategoryMapperTest, TestItReturnsTypeIfKnown)
        {
            this->mapper.AddCategoryMapping("B733", "LM");
            EXPECT_TRUE(this->mapper.GetCategoryForAircraftType("B733") == "LM");
        }

        TEST_F(WakeCategoryMapperTest, TestItReturnsNoTypeIfKNowKnown)
        {
            EXPECT_TRUE(this->mapper.GetCategoryForAircraftType("B733") == "");
        }
    }  // namespace Wake
}  // namespace UKControllerPluginTest
