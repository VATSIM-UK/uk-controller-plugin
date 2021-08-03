#include "pch/pch.h"
#include "login/LoginModule.h"
#include "bootstrap/PersistenceContainer.h"
#include "timedevent/TimedEventCollection.h"
#include "controller/ControllerStatusEventHandlerCollection.h"

using UKControllerPlugin::Controller::LoginModule;
using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::TimedEvent::TimedEventCollection;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;

using ::testing::Test;

namespace UKControllerPluginTest {
    namespace Controller {

        class LoginModuleTest : public Test
        {
            public:
                void SetUp()
                {
                    container.timedHandler = std::make_unique<TimedEventCollection>();
                    container.controllerHandler = std::make_unique<ControllerStatusEventHandlerCollection>();
                    LoginModule::BootstrapPlugin(container);
                }

                PersistenceContainer container;
        };

        TEST_F(LoginModuleTest, BootstrapPluginSetsContainerLogin)
        {
            EXPECT_EQ(1, this->container.login->loggedIn);
        }

        TEST_F(LoginModuleTest, BootstrapPluginRegistersTimedEventForEveryTwoSeconds)
        {
            EXPECT_EQ(1, this->container.timedHandler->CountHandlers());
            EXPECT_EQ(1, this->container.timedHandler->CountHandlersForFrequency(2));
        }

        TEST_F(LoginModuleTest, BootstrapPluginRegistersControllerStatusEvents)
        {
            EXPECT_EQ(1, this->container.controllerHandler->CountHandlers());
        }
    }  // namespace Controller
}  // namespace UKControllerPluginTest
