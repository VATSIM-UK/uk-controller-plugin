#include "pch/pch.h"
#include "controller/ControllerPositionCollectionFactory.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerPosition.h"
#include "mock/MockDependencyLoader.h"

using UKControllerPlugin::Controller::ControllerPosition;
using UKControllerPlugin::Controller::ControllerPositionCollection;
using UKControllerPlugin::Controller::ControllerPositionCollectionFactory;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

using ::testing::ElementsAre;
using ::testing::Test;
using ::testing::NiceMock;
using ::testing::Return;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerPositionCollectionFactoryTest : public Test
        {
            public:

                NiceMock<MockDependencyLoader> dependency;
        };

        TEST_F(ControllerPositionCollectionFactoryTest, ReturnsEmptyCollectionIfNoDependencyFound)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS", nlohmann::json::object()))
                .WillByDefault(Return(nlohmann::json::object()));
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST_F(ControllerPositionCollectionFactoryTest, ReturnsEmptyCollectionIfInvalidJson)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS", nlohmann::json::object()))
                .WillByDefault(Return(nlohmann::json::array()));
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST_F(ControllerPositionCollectionFactoryTest, AddsDoesntAddControllerIfNotValid)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS", nlohmann::json::object()))
                .WillByDefault(Return("{\"EGAA_GND\": {\"frequency\": \"test\", \"top-down\" : [\"EGAA\"]}}"_json));
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            EXPECT_EQ(0, collection->GetSize());
        }

        TEST_F(ControllerPositionCollectionFactoryTest, AddsControllerIfValid)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS", nlohmann::json::object()))
                .WillByDefault(Return("{\"EGAA_GND\": {\"frequency\": 121.750, \"top-down\" : [\"EGAA\"]}}"_json));
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            ControllerPosition position = collection->FetchPositionByCallsign("EGAA_GND");
            EXPECT_EQ(0, position.GetCallsign().compare("EGAA_GND"));
            EXPECT_EQ(121.750, position.GetFrequency());
            EXPECT_EQ(0, position.GetType().compare("GND"));
            EXPECT_THAT(position.GetTopdown(), ElementsAre("EGAA"));
        }

        TEST_F(ControllerPositionCollectionFactoryTest, AddsControllerIfValidWithZeroEndingFrequency)
        {
            ON_CALL(this->dependency, LoadDependency("DEPENDENCY_CONTROLLER_POSITIONS", nlohmann::json::object()))
                .WillByDefault(Return("{\"EGAA_GND\": {\"frequency\": 121.000, \"top-down\" : [\"EGAA\"]}}"_json));
            std::shared_ptr<ControllerPositionCollection> collection = ControllerPositionCollectionFactory::Create(
                dependency
            );
            ControllerPosition position = collection->FetchPositionByCallsign("EGAA_GND");
            EXPECT_EQ(0, position.GetCallsign().compare("EGAA_GND"));
            EXPECT_EQ(121.000, position.GetFrequency());
            EXPECT_EQ(0, position.GetType().compare("GND"));
            EXPECT_THAT(position.GetTopdown(), ElementsAre("EGAA"));
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
