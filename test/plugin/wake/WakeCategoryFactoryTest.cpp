#include "wake/ArrivalWakeInterval.h"
#include "wake/DepartureWakeInterval.h"
#include "wake/WakeCategory.h"
#include "wake/WakeCategoryFactory.h"

using UKControllerPlugin::Wake::CategoryValid;
using UKControllerPlugin::Wake::WakeCategory;
using UKControllerPlugin::Wake::WakeCategoryFromJson;

namespace UKControllerPluginTest::Wake {
    class WakeCategoryFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetCategory(nlohmann::json override = nlohmann::json::object(), std::string removeKey = "") -> nlohmann::json
        {
            nlohmann::json category{
                {"id", 123},
                {"code", "LM"},
                {"description", "Lower Medium"},
                {"relative_weighting", 15},
                {"subsequent_departure_intervals",
                 nlohmann::json::array(
                     {{{"id", 1}, {"interval", 120}, {"interval_unit", "s"}, {"intermediate", false}},
                      {{"id", 2}, {"interval", 180}, {"interval_unit", "s"}, {"intermediate", true}}})},
                {"subsequent_arrival_intervals",
                 nlohmann::json::array({{{"id", 1}, {"interval", 4.5}}, {{"id", 2}, {"interval", 5.0}}})}};

            category.update(override);

            if (!removeKey.empty()) {
                category.erase(category.find(removeKey));
            }

            return category;
        }
    };

    TEST_F(WakeCategoryFactoryTest, ItReturnsCategory)
    {
        const auto category = WakeCategoryFromJson(GetCategory());
        EXPECT_EQ(123, category->Id());
        EXPECT_EQ("LM", category->Code());
        EXPECT_EQ("Lower Medium", category->Description());
        EXPECT_EQ(15, category->RelativeWeighting());
        EXPECT_EQ(2, category->SubsequentDepartureIntervals().size());
        EXPECT_EQ(1, category->SubsequentDepartureIntervals().front()->subsequentWakeCategoryId);
        EXPECT_EQ(120, category->SubsequentDepartureIntervals().front()->intervalValue);
        EXPECT_EQ("s", category->SubsequentDepartureIntervals().front()->intervalUnit);
        EXPECT_FALSE(category->SubsequentDepartureIntervals().front()->intervalIsIntermediate);
        EXPECT_EQ(2, category->SubsequentDepartureIntervals().back()->subsequentWakeCategoryId);
        EXPECT_EQ(180, category->SubsequentDepartureIntervals().back()->intervalValue);
        EXPECT_EQ("s", category->SubsequentDepartureIntervals().back()->intervalUnit);
        EXPECT_TRUE(category->SubsequentDepartureIntervals().back()->intervalIsIntermediate);
        EXPECT_EQ(2, category->SubsequentArrivalIntervals().size());
        EXPECT_EQ(1, category->SubsequentArrivalIntervals().front()->subsequentWakeCategoryId);
        EXPECT_FLOAT_EQ(4.5, category->SubsequentArrivalIntervals().front()->intervalValue);
        EXPECT_EQ(2, category->SubsequentArrivalIntervals().back()->subsequentWakeCategoryId);
        EXPECT_FLOAT_EQ(5.0, category->SubsequentArrivalIntervals().back()->intervalValue);
    }

    TEST_F(WakeCategoryFactoryTest, ItIgnoresBadDepartureIntervals)
    {
        const nlohmann::json intervals = {
            {"subsequent_departure_intervals",
             nlohmann::json::array(
                 {{{"id", "abc"}, {"interval", 120}, {"interval_unit", "s"}, {"intermediate", false}}})}};

        const auto category = WakeCategoryFromJson(GetCategory(intervals));
        EXPECT_EQ(123, category->Id());
        EXPECT_EQ("LM", category->Code());
        EXPECT_EQ("Lower Medium", category->Description());
        EXPECT_EQ(15, category->RelativeWeighting());
        EXPECT_EQ(0, category->SubsequentDepartureIntervals().size());
    }

    TEST_F(WakeCategoryFactoryTest, ItIgnoresBadArrivalIntervals)
    {
        const nlohmann::json intervals = {
            {"subsequent_arrival_intervals", nlohmann::json::array({{{"id", "abc"}, {"interval", 1.5}}})}};

        const auto category = WakeCategoryFromJson(GetCategory(intervals));
        EXPECT_EQ(123, category->Id());
        EXPECT_EQ("LM", category->Code());
        EXPECT_EQ("Lower Medium", category->Description());
        EXPECT_EQ(15, category->RelativeWeighting());
        EXPECT_EQ(2, category->SubsequentDepartureIntervals().size());
        EXPECT_EQ(0, category->SubsequentArrivalIntervals().size());
    }

    TEST_F(WakeCategoryFactoryTest, ItReturnsNullptrBadCategory)
    {
        const auto category = WakeCategoryFromJson(GetCategory(nlohmann::json::object(), "id"));
        EXPECT_EQ(nullptr, category);
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsValid)
    {
        EXPECT_TRUE(CategoryValid(GetCategory()));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidIdMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "id")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidIdNotInteger)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"id", "abc"}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidCodeMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "code")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidCodeNotString)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"code", 123}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidDescriptionMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "description")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidDescriptionNotString)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"description", 123}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidRelativeWeightingMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "relative_weighting")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidRelativeWeightingNotInteger)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"relative_weighting", "abc"}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidDepartureIntervalsMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "subsequent_departure_intervals")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidDepartureIntervalsNotArray)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"subsequent_departure_intervals", "abc"}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidArrivalIntervalsMissing)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object(), "subsequent_arrival_intervals")));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidArrivalIntervalsNotArray)
    {
        EXPECT_FALSE(CategoryValid(GetCategory(nlohmann::json::object({{"subsequent_arrival_intervals", "abc"}}))));
    }

    TEST_F(WakeCategoryFactoryTest, CategoryIsInvalidIfNotObject)
    {
        EXPECT_FALSE(CategoryValid(nlohmann::json::array()));
    }
} // namespace UKControllerPluginTest::Wake
