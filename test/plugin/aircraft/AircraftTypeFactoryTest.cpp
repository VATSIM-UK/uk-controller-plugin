#include "aircraft/AircraftType.h"
#include "aircraft/AircraftTypeFactory.h"

using UKControllerPlugin::Aircraft::FromJson;
using UKControllerPlugin::Aircraft::JsonValid;

namespace UKControllerPluginTest::Aircraft {
    class AircraftTypeFactoryTest : public testing::Test
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

    TEST_F(AircraftTypeFactoryTest, TypeIsValid)
    {
        EXPECT_TRUE(JsonValid(GetType()));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidDataNotObject)
    {
        EXPECT_FALSE(JsonValid(nlohmann::json::array()));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidIdMissing)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object(), "id")));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidIdNotInteger)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object({{"id", "abc"}}))));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidIcaoCodeMissing)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object(), "icao_code")));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidIcaoCodeNotString)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object({{"icao_code", 123}}))));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidWakeCategoriesMissing)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object(), "wake_categories")));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidWakeCategoriesNotArray)
    {
        EXPECT_FALSE(JsonValid(GetType(nlohmann::json::object({{"wake_categories", "abc"}}))));
    }

    TEST_F(AircraftTypeFactoryTest, TypeIsInvalidWakeCategoryNotString)
    {
        EXPECT_FALSE(
            JsonValid(GetType(nlohmann::json::object({{"wake_categories", nlohmann::json::array({1, 2, "c"})}}))));
    }

    TEST_F(AircraftTypeFactoryTest, ItReturnsNullptrInvalidData)
    {
        const auto aircraftType = FromJson(GetType(nlohmann::json::object({{"id", "abc"}})));
        EXPECT_EQ(nullptr, aircraftType);
    }

    TEST_F(AircraftTypeFactoryTest, ItReturnsAircraftType)
    {
        const auto aircraftType = FromJson(GetType());
        EXPECT_EQ(123, aircraftType->Id());
        EXPECT_EQ("B738", aircraftType->IcaoCode());
        EXPECT_EQ(std::set<int>({1, 2, 3}), aircraftType->WakeCategories());
    }
} // namespace UKControllerPluginTest::Aircraft
