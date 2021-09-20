#include "bootstrap/PersistenceContainer.h"
#include "controller/ActiveCallsignCollection.h"
#include "controller/ControllerBootstrap.h"
#include "controller/ControllerPositionCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using testing::NiceMock;
using testing::Test;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPluginTest::Dependency::MockDependencyLoader;

namespace UKControllerPluginTest::Controller {

    class ControllerBootstrapTest : public Test
    {
        public:
        ControllerBootstrapTest()
        {
            container.controllerHandler = std::make_unique<ControllerStatusEventHandlerCollection>();
        }

        NiceMock<MockDependencyLoader> dependency;
        PersistenceContainer container;
    };

    TEST_F(ControllerBootstrapTest, ItSetsUpControllerCollection)
    {
        UKControllerPlugin::Controller::BootstrapPlugin(container, dependency);
        EXPECT_EQ(0, container.controllerPositions->GetSize());
    }

    TEST_F(ControllerBootstrapTest, ItSetsUpActiveCallsigns)
    {
        UKControllerPlugin::Controller::BootstrapPlugin(container, dependency);
        EXPECT_EQ(0, container.activeCallsigns->CountHandlers());
    }

    TEST_F(ControllerBootstrapTest, ItRegistersForControllerEvents)
    {
        UKControllerPlugin::Controller::BootstrapPlugin(container, dependency);
        EXPECT_EQ(1, container.controllerHandler->CountHandlers());
    }
} // namespace UKControllerPluginTest::Controller
