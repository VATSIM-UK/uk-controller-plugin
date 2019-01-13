#include "pch/pch.h"
#include "airfield/AirfieldCollection.h"
#include "airfield/AirfieldCollectionFactory.h"
#include "dependency/DependencyCache.h"
#include "airfield/Airfield.h"

using UKControllerPlugin::Airfield::AirfieldCollection;
using UKControllerPlugin::Airfield::AirfieldCollectionFactory;
using UKControllerPlugin::Dependency::DependencyCache;
using UKControllerPlugin::Airfield::Airfield;
using ::testing::ElementsAre;

namespace UKControllerPluginTest {
    namespace Airfield {

        TEST(AirfieldCollectionFactory, HasCorrectDependencyFile)
        {
            DependencyCache dependency;
            EXPECT_TRUE("airfield-ownership.json" == AirfieldCollectionFactory::requiredDependency);
        }

        TEST(AirfieldCollectionFactory, CreateReturnsEmptyColletionIfDependencyMissing)
        {
            DependencyCache dependency;
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST(AirfieldCollectionFactory, CreateReturnsEmptyColletionIfDependencyInvalidJson)
        {
            DependencyCache dependency;
            dependency.AddDependency(AirfieldCollectionFactory::requiredDependency, "{not valid : json}");
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST(AirfieldCollectionFactory, AddsAllAirfields)
        {
            DependencyCache dependency;
            std::string dependencyJson = "{\"EGKK\":[\"EGKK_DEL\"], \"EGLL\" : [\"EGLL_DEL\"]}";
            dependency.AddDependency(AirfieldCollectionFactory::requiredDependency, dependencyJson);
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);
            EXPECT_EQ(2, collection->GetSize());
        }

        TEST(AirfieldCollectionFactory, AddsCorrectAirfields)
        {
            DependencyCache dependency;
            std::string dependencyJson = "{\"EGKK\":[\"EGKK_DEL\"], \"EGLL\" : [\"EGLL_DEL\"]}";
            dependency.AddDependency(AirfieldCollectionFactory::requiredDependency, dependencyJson);
            std::unique_ptr<const AirfieldCollection> collection = AirfieldCollectionFactory::Create(dependency);

            EXPECT_THAT(collection->FetchAirfieldByIcao("EGKK").GetOwnershipPresedence(), ElementsAre("EGKK_DEL"));
            EXPECT_THAT(collection->FetchAirfieldByIcao("EGLL").GetOwnershipPresedence(), ElementsAre("EGLL_DEL"));
        }
    }  // namespace Airfield
}  // namespace UKControllerPluginTest
