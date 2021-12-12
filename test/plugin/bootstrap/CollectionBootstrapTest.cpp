#include "bootstrap/CollectionBootstrap.h"
#include "bootstrap/PersistenceContainer.h"
#include "flightplan/FlightPlanEventHandlerCollection.h"
#include "flightplan/StoredFlightplanCollection.h"
#include "timedevent/TimedEventCollection.h"

using ::testing::NiceMock;
using ::testing::Test;
using UKControllerPlugin::Bootstrap::CollectionBootstrap;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Flightplan::FlightPlanEventHandlerCollection;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Bootstrap {

    class CollectionBootstrapTest : public Test
    {
        public:
        void SetUp() override
        {
            container.timedHandler = std::make_unique<TimedEventCollection>();
            container.flightplanHandler = std::make_unique<FlightPlanEventHandlerCollection>();
        }
        PersistenceContainer container;
        NiceMock<MockDependencyLoader> dependency;
    };

    TEST_F(CollectionBootstrapTest, BootstrapPluginCreatesFlightplans)
    {
        CollectionBootstrap::BootstrapPlugin(this->container, this->dependency);
        EXPECT_NO_THROW(container.flightplans->cend());
    }
} // namespace UKControllerPluginTest::Bootstrap
