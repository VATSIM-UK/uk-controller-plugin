#include "wake/WakeCategory.h"
#include "wake/WakeScheme.h"
#include "wake/WakeSchemeFactory.h"
#include "wake/WakeSchemeCollection.h"
#include "wake/WakeSchemeCollectionFactory.h"

using UKControllerPlugin::Wake::CollectionFromDependency;
using UKControllerPlugin::Wake::DependencyValid;
using UKControllerPlugin::Wake::WakeScheme;

namespace UKControllerPluginTest::Wake {
    class WakeSchemeCollectionFactoryTest : public testing::Test
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
                       {"subsequent_departure_intervals", nlohmann::json::array()}},
                      {{"id", 2},
                       {"code", "H"},
                       {"description", "Heavy"},
                       {"relative_weighting", 20},
                       {"subsequent_departure_intervals", nlohmann::json::array()}}})}};

            scheme.update(override);

            if (!removeKey.empty()) {
                scheme.erase(scheme.find(removeKey));
            }

            return scheme;
        }
    };

    TEST_F(WakeSchemeCollectionFactoryTest, DependencyIsValidIfArray)
    {
        EXPECT_TRUE(DependencyValid(nlohmann::json::array()));
    }

    TEST_F(WakeSchemeCollectionFactoryTest, DependencyIsInvalidIfNotArray)
    {
        EXPECT_FALSE(DependencyValid(nlohmann::json::object()));
    }

    TEST_F(WakeSchemeCollectionFactoryTest, ItReturnsEmptyCollectionOnInvalidDependency)
    {
        const auto collection = CollectionFromDependency(nlohmann::json::object());
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(WakeSchemeCollectionFactoryTest, ItIgnoresInvalidSchemesForCollection)
    {
        const auto dependency = nlohmann::json::array({
            GetScheme({{"id", "abc"}}),
            GetScheme({{"key", 123}}),
        });

        const auto collection = CollectionFromDependency(dependency);
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(WakeSchemeCollectionFactoryTest, ItLoadsSchemesIntoCollection)
    {
        const auto dependency = nlohmann::json::array({
            GetScheme(),
            GetScheme({{"id", 2}}),
        });

        const auto collection = CollectionFromDependency(dependency);
        EXPECT_EQ(2, collection->Count());
    }
} // namespace UKControllerPluginTest::Wake
