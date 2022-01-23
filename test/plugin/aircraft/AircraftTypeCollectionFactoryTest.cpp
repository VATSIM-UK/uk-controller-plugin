#include "aircraft/AircraftType.h"
#include "aircraft/AircraftTypeCollection.h"
#include "aircraft/AircraftTypeCollectionFactory.h"

using UKControllerPlugin::Aircraft::CollectionFromDependency;
using UKControllerPlugin::Aircraft::DependencyValid;

namespace UKControllerPluginTest::Aircraft {
    class AircraftTypeCollectionFactoryTest : public testing::Test
    {
        public:
        [[nodiscard]] static auto
        GetType(nlohmann::json override = nlohmann::json::object(), std::string removeKey = "") -> nlohmann::json
        {
            nlohmann::json type{
                {"id", 123},
                {"icao_code", "B738"},
                {"wake_categories", nlohmann::json::array({1, 2, 3})},
            };

            type.update(override);

            if (!removeKey.empty()) {
                type.erase(type.find(removeKey));
            }

            return type;
        }
    };

    TEST_F(AircraftTypeCollectionFactoryTest, DependencyIsValidIfArrray)
    {
        EXPECT_TRUE(DependencyValid(nlohmann::json::array()));
    }

    TEST_F(AircraftTypeCollectionFactoryTest, DependencyIsInvalidIfNotArrray)
    {
        EXPECT_FALSE(DependencyValid(nlohmann::json::object()));
    }

    TEST_F(AircraftTypeCollectionFactoryTest, ItReturnsEmptyCollectionDependencyInvalid)
    {
        const auto collection = CollectionFromDependency(nlohmann::json::object());
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(AircraftTypeCollectionFactoryTest, ItIgnoresBadAircraftTypes)
    {
        nlohmann::json data = nlohmann::json::array({
            GetType(nlohmann::json::object({{"id", "abc"}})),
            GetType(nlohmann::json::object({{"id", "def"}})),
        });

        const auto collection = CollectionFromDependency(data);
        EXPECT_EQ(0, collection->Count());
    }

    TEST_F(AircraftTypeCollectionFactoryTest, ItLoadsAircraftTypesToCollection)
    {
        nlohmann::json data = nlohmann::json::array({
            GetType(),
            GetType(nlohmann::json::object({{"id", 2}, {"icao_code", "A320"}})),
        });

        const auto collection = CollectionFromDependency(data);
        EXPECT_EQ(2, collection->Count());
        EXPECT_EQ(123, collection->GetByIcaoCode("B738")->Id());
        EXPECT_EQ(2, collection->GetByIcaoCode("A320")->Id());
    }
} // namespace UKControllerPluginTest::Aircraft
