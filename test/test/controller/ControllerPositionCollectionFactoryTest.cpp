#include "pch/pch.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "dependency/DependencyCache.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPlugin::Dependency::DependencyCache;

using ::testing::ElementsAre;

namespace UKControllerPluginTest {
    namespace Controller {

        TEST(ControllerPositionCollectionFactory, HasCorrectDependencyFile)
        {
            DependencyCache dependency;
            EXPECT_TRUE("controller-positions.json" == ControllerPositionCollectionFactory::requiredDependency);
        }

        TEST(ControllerPositionCollectionFactory, ReturnsEmptyCollectionIfNoDependencyFound)
        {
            DependencyCache dependency;
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST(ControllerPositionCollectionFactory, ReturnsEmptyCollectionIfInvalidJson)
        {
            DependencyCache dependency;
            dependency.AddDependency(ControllerPositionCollectionFactory::requiredDependency, "{\"not\": valid json}");
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST(ControllerPositionCollectionFactory, AddsControllerIfValid)
        {
            DependencyCache dependency;
            dependency.AddDependency(
                ControllerPositionCollectionFactory::requiredDependency,
                "{\"EGAA_GND\": {\"frequency\": 121.750, \"top-down\" : [\"EGAA\"]}}"
            );
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            ControllerPosition position = collection->FetchPositionByCallsign("EGAA_GND");
            EXPECT_EQ(0, position.GetCallsign().compare("EGAA_GND"));
            EXPECT_EQ(121.750, position.GetFrequency());
            EXPECT_EQ(0, position.GetType().compare("GND"));
            EXPECT_THAT(position.GetTopdown(), ElementsAre("EGAA"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
