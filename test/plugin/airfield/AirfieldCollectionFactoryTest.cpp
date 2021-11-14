#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "airfield/AirfieldModel.h"

using ::testing::ElementsAre;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;
using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::CreateAirfieldColletion;
using UKControllerPlugin::Airfield::AirfieldModel;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest {
    namespace Airfield {

        class AirfieldCollectionFactoryTest : public Test
        {
            public:
            NiceMock<MockDependencyLoader> dependency;
        };

        TEST_F(AirfieldCollectionFactoryTest, CreateReturnsEmptyColletionIfDependencyMissing)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_AIRFIELD_OWNERSHIP", nlohmann::json::object()))
                .WillByDefault(Return(nlohmann::json::object()));
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST_F(AirfieldCollectionFactoryTest, CreateReturnsEmptyColletionIfDependencyInvalidJson)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_AIRFIELD_OWNERSHIP", nlohmann::json::object()))
                .WillByDefault(Return(nlohmann::json::array()));
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST_F(AirfieldCollectionFactoryTest, AddsAllAirfields)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_AIRFIELD_OWNERSHIP", nlohmann::json::object()))
                .WillByDefault(Return("{\"EGKK\":[\"EGKK_DEL\"], \"EGLL\" : [\"EGLL_DEL\"]}"_json));
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(2, collection->GetSize());
        }

        TEST_F(AirfieldCollectionFactoryTest, AddsCorrectAirfields)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_AIRFIELD_OWNERSHIP", nlohmann::json::object()))
                .WillByDefault(Return("{\"EGKK\":[\"EGKK_DEL\"], \"EGLL\" : [\"EGLL_DEL\"]}"_json));
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);

            EXPECT_THAT(collection->FetchAirfieldByIcao("EGKK").GetOwnershipPresedence(), ElementsAre("EGKK_DEL"));
            EXPECT_THAT(collection->FetchAirfieldByIcao("EGLL").GetOwnershipPresedence(), ElementsAre("EGLL_DEL"));
        }
    } // namespace Airfield
} // namespace UKControllerPluginTest
