#include "wake/WakeCategory.h"
#include "wake/WakeScheme.h"
#include "wake/WakeSchemeFactory.h"

using UKControllerPlugin::Wake::SchemeValid;
using UKControllerPlugin::Wake::WakeScheme;
using UKControllerPlugin::Wake::WakeSchemeFromJson;

namespace UKControllerPluginTest::Wake {
    class WakeSchemeFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetScheme(nlohmann::json override = nlohmann::json::object(), std::string removeKey = "") -> nlohmann::json
        {
            nlohmann::json scheme{
                {"id", 123},
                {"key", "UK"},
                {"name", "UK Wake Scheme"},
                {"categories",
                 nlohmann::json::array(
                     {{{"id", 1},
                       {"code", "LM"},
                       {"description", "Lower Medium"},
                       {"relative_weighting", 5},
                       {"subsequent_departure_intervals", nlohmann::json::array()},
                       {"subsequent_arrival_intervals", nlohmann::json::array()}},
                      {{"id", 2},
                       {"code", "H"},
                       {"description", "Heavy"},
                       {"relative_weighting", 20},
                       {"subsequent_departure_intervals", nlohmann::json::array()},
                       {"subsequent_arrival_intervals", nlohmann::json::array()}}})}};

            scheme.update(override);

            if (!removeKey.empty()) {
                scheme.erase(scheme.find(removeKey));
            }

            return scheme;
        }
    };

    TEST_F(WakeSchemeFactoryTest, ItReturnsAScheme)
    {
        const auto scheme = WakeSchemeFromJson(GetScheme());
        EXPECT_EQ(123, scheme->Id());
        EXPECT_EQ("UK", scheme->Key());
        EXPECT_EQ("UK Wake Scheme", scheme->Name());
        EXPECT_EQ(2, scheme->Categories().size());
        EXPECT_EQ(1, scheme->Categories().front()->Id());
        EXPECT_EQ(2, scheme->Categories().back()->Id());
    }

    TEST_F(WakeSchemeFactoryTest, ItIgnoresInvalidCategories)
    {
        nlohmann::json categories{
            {"categories",
             nlohmann::json::array(
                 {{{"id", "abc"}, // Invalid
                   {"code", "LM"},
                   {"description", "Lower Medium"},
                   {"relative_weighting", 5},
                   {"subsequent_departure_intervals", nlohmann::json::array()},
                   {"subsequent_arrival_intervals", nlohmann::json::array()}},
                  {{"id", 2},
                   {"code", "LM"},
                   {"description", "Lower Medium"},
                   {"relative_weighting", 5},
                   {"subsequent_departure_intervals", nlohmann::json::array()},
                   {"subsequent_arrival_intervals", nlohmann::json::array()}}})}};

        const auto scheme = WakeSchemeFromJson(GetScheme(categories));
        EXPECT_EQ(123, scheme->Id());
        EXPECT_EQ("UK", scheme->Key());
        EXPECT_EQ("UK Wake Scheme", scheme->Name());
        EXPECT_EQ(1, scheme->Categories().size());
        EXPECT_EQ(2, scheme->Categories().front()->Id());
    }

    TEST_F(WakeSchemeFactoryTest, ItReturnsNullPtrOnBadScheme)
    {
        const auto scheme = WakeSchemeFromJson(GetScheme(nlohmann::json::object(), "id"));
        EXPECT_EQ(nullptr, scheme);
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsValid)
    {
        EXPECT_TRUE(SchemeValid(GetScheme()));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidNotObject)
    {
        EXPECT_FALSE(SchemeValid(nlohmann::json::array()));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidIdMissing)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object(), "id")));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidIdNotInteger)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object({{"id", "abc"}}))));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidKeyMissing)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object(), "key")));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidKeyNotString)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object({{"key", 123}}))));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidNameMissing)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object(), "name")));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidNameNotString)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object({{"name", 123}}))));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidCategoriesMissing)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object(), "categories")));
    }

    TEST_F(WakeSchemeFactoryTest, SchemeIsInvalidCategoriesNotArray)
    {
        EXPECT_FALSE(SchemeValid(GetScheme(nlohmann::json::object({{"categories", 123}}))));
    }
} // namespace UKControllerPluginTest::Wake
