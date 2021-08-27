#include "bootstrap/PersistenceContainer.h"
#include "controller/ControllerStatusEventHandlerCollection.h"
#include "login/Login.h"
#include "login/LoginModule.h"
#include "timedevent/TimedEventCollection.h"

using UKControllerPlugin::Bootstrap::PersistenceContainer;
using UKControllerPlugin::Controller::ControllerStatusEventHandlerCollection;
using UKControllerPlugin::Controller::LoginModule;
using UKControllerPlugin::TimedEvent::TimedEventCollection;

using ::testing::Test;

namespace UKControllerPluginTest::Controller {

    class LoginModuleTest : public Test
    {
        public:
        void SetUp() override
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
} // namespace UKControllerPluginTest::Controller
