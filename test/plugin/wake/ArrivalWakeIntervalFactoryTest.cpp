#include "wake/ArrivalWakeInterval.h"
#include "wake/ArrivalWakeIntervalFactory.h"

using UKControllerPlugin::Wake::ArrivalIntervalFromJson;
using UKControllerPlugin::Wake::ArrivalIntervalValid;

namespace UKControllerPluginTest::Wake {
    class ArrivalWakeIntervalFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetInterval(nlohmann::json override = nlohmann::json::object(), std::string removeKey = "") -> nlohmann::json
        {
            nlohmann::json interval{
                {"id", 123},
                {"interval", 4.5},
            };

            interval.update(override);

            if (!removeKey.empty()) {
                interval.erase(interval.find(removeKey));
            }

            return interval;
        }
    };

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsValid)
    {
        EXPECT_TRUE(ArrivalIntervalValid(GetInterval()));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsInvalidIdMissing)
    {
        EXPECT_FALSE(ArrivalIntervalValid(GetInterval(nlohmann::json::object(), "id")));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsInvalidIdNotInteger)
    {
        EXPECT_FALSE(ArrivalIntervalValid(GetInterval(nlohmann::json::object({{"id", "abc"}}))));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsInvalidIntervalMissing)
    {
        EXPECT_FALSE(ArrivalIntervalValid(GetInterval(nlohmann::json::object(), "interval")));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsInvalidIntervalNotNumber)
    {
        EXPECT_FALSE(ArrivalIntervalValid(GetInterval(nlohmann::json::object({{"interval", "abc"}}))));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, IntervalIsInvalidDataNotObject)
    {
        EXPECT_FALSE(ArrivalIntervalValid(nlohmann::json::array()));
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, ItReturnsNullptrInvalidInterval)
    {
        const auto interval = ArrivalIntervalFromJson(GetInterval(nlohmann::json::object({{"id", "abc"}})));
        EXPECT_EQ(nullptr, interval);
    }

    TEST_F(ArrivalWakeIntervalFactoryTest, ItReturnsInterval)
    {
        const auto interval = ArrivalIntervalFromJson(GetInterval());
        EXPECT_EQ(123, interval->SubsequentCategory());
        EXPECT_FLOAT_EQ(4.5, interval->Value());
    }
} // namespace UKControllerPluginTest::Wake
