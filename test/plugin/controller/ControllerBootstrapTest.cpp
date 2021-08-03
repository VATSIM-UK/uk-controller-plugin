#include "pch/pch.h"
#include "controller/ControllerBootstrap.h"
#include "mock/MockDependencyLoader.h"
#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPluginTest::Dependency::MockDependencyLoader;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using testing::Test;
using testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Controller {

        class ControllerBootstrapTest : public Test
        {
            public:
                ControllerBootstrapTest()
                {
                    container.controllerHandler.reset(new ControllerStatusEventHandlerCollection);
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
    }  // namespace Controller
}  // namespace UKControllerPluginTest
