#include "wake/DepartureWakeInterval.h"
#include "wake/DepartureWakeIntervalFactory.h"

using UKControllerPlugin::Wake::IntervalFromJson;
using UKControllerPlugin::Wake::IntervalUnitValid;
using UKControllerPlugin::Wake::IntervalValid;

namespace UKControllerPluginTest::Wake {
    class DepartureWakeIntervalFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetInterval(nlohmann::json override = nlohmann::json::object(), std::string removeKey = "") -> nlohmann::json
        {
            nlohmann::json interval{
                {"id", 123},
                {"interval", 120},
                {"interval_unit", "s"},
                {"intermediate", false},
            };

            interval.update(override);

            if (!removeKey.empty()) {
                interval.erase(interval.find(removeKey));
            }

            return interval;
        }
    };

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalUnitIsValidNauticalMiles)
    {
        EXPECT_TRUE(IntervalUnitValid("nm"));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalUnitIsValidSeconds)
    {
        EXPECT_TRUE(IntervalUnitValid("s"));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalUnitIsInvalid)
    {
        EXPECT_FALSE(IntervalUnitValid("min"));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsValid)
    {
        EXPECT_TRUE(IntervalValid(GetInterval()));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIdMissing)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object(), "id")));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIdNotInteger)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object({{"id", "abc"}}))));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntervalMissing)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object(), "interval")));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntervalNotInteger)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object({{"interval", "abc"}}))));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntervalUnitMissing)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object(), "interval_unit")));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntervalUnitNotString)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object({{"interval_unit", 123}}))));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntervalUnitNotValidUnit)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object({{"interval_unit", "mm"}}))));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntermediateMissing)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object(), "intermediate")));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidIntermediateNotBoolean)
    {
        EXPECT_FALSE(IntervalValid(GetInterval(nlohmann::json::object({{"intermediate", "abc"}}))));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, IntervalIsInvalidDataNotObject)
    {
        EXPECT_FALSE(IntervalValid(nlohmann::json::array()));
    }

    TEST_F(DepartureWakeIntervalFactoryTest, ItReturnsNullptrInvalidInterval)
    {
        const auto interval = IntervalFromJson(GetInterval(nlohmann::json::object({{"intermediate", "abc"}})));
        EXPECT_EQ(nullptr, interval);
    }

    TEST_F(DepartureWakeIntervalFactoryTest, ItReturnsInterval)
    {
        const auto interval = IntervalFromJson(GetInterval());
        EXPECT_EQ(123, interval->subsequentWakeCategoryId);
        EXPECT_EQ(120, interval->intervalValue);
        EXPECT_EQ("s", interval->intervalUnit);
        EXPECT_EQ(false, interval->intervalIsIntermediate);
    }
} // namespace UKControllerPluginTest::Wake
