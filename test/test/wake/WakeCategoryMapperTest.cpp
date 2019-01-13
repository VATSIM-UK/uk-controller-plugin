#include "pch/pch.h"
#include "wake/WakeCategoryMapper.h"
#include "mock/MockEuroScopeCFlightplanInterface.h"

using UKControllerPlugin::Wake::WakeCategoryMapper;
using UKControllerPluginTest::Euroscope::MockEuroScopeCFlightPlanInterface;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Wake {
        class WakeCategoryMapperTest : public Test
        {
            public:
                NiceMock<MockEuroScopeCFlightPlanInterface> flightplan;
                WakeCategoryMapper mapper;
        };

        TEST_F(WakeCategoryMapperTest, TestItStartsEmpty)
        {
            EXPECT_EQ(0, this->mapper.Count());
        }

        TEST_F(WakeCategoryMapperTest, TestItAddsAMapping)
        {
            this->mapper.AddTypeMapping("B733", "LM");
            EXPECT_EQ(1, this->mapper.Count());
        }

        TEST_F(WakeCategoryMapperTest, TestItMapsIfTypeMatch)
        {
            ON_CALL(this->flightplan, GetAircraftType())
                .WillByDefault(Return("B733"));

            this->mapper.AddTypeMapping("B733", "LM");
            EXPECT_TRUE(this->mapper.MapFlightplanToCategory(this->flightplan) == "LM");
        }

        TEST_F(WakeCategoryMapperTest, TestItReturnsIcaoIfNoMap)
        {
            ON_CALL(this->flightplan, GetAircraftType())
                .WillByDefault(Return("B733"));

            ON_CALL(this->flightplan, GetIcaoWakeCategory())
                .WillByDefault(Return("M"));

            this->mapper.AddTypeMapping("B744", "H");
            EXPECT_TRUE(this->mapper.MapFlightplanToCategory(this->flightplan) == "M");
        }

    }  // namespace Wake
}  // namespace UKControllerPluginTest
