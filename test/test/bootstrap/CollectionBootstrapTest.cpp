#include "pch/pch.h"
#include "bootstrap/CollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "mock/MockDependencyLoader.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Bootstrap::CollectionBootstrap;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Bootstrap {

        class CollectionBootstrapTest : public Test
        {
            public:
                void SetUp()
                {
                    container.timedHandler.reset(new TimedEventCollection);
                    container.flightplanHandler.reset(new FlightPlanEventHandlerCollection);
                }
                PersistenceContainer container;
                NiceMock<MockDependencyLoader> dependency;
        };

        TEST_F(CollectionBootstrapTest, BootstrapPluginCreatesAirfields)
        {
            CollectionBootstrap::BootstrapPlugin(this->container, this->dependency);
            EXPECT_NO_THROW(container.airfields->GetSize());
        }

        TEST_F(CollectionBootstrapTest, BootstrapPluginCreatesAirfieldOwnership)
        {
            CollectionBootstrap::BootstrapPlugin(this->container, this->dependency);
            EXPECT_NO_THROW(container.airfieldOwnership->Flush());
        }

        TEST_F(CollectionBootstrapTest, BootstrapPluginCreatesFlightplans)
        {
            CollectionBootstrap::BootstrapPlugin(this->container, this->dependency);
            EXPECT_NO_THROW(container.flightplans->cend());
        }
    }  // namespace Bootstrap
}  // namespace UKControllerPluginTest
